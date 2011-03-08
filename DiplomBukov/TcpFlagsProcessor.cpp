#include "TcpFlagsProcessor.h"
#include <algorithm>

using namespace DiplomBukov;

TcpFlagsProcessor::TcpFlagsProcessor(IProcessorPtr Connector)
    : connectionStatus(CLOSED)
    , client()
    , server()
{
    setNextProcessor(Connector);
}

IProcessorPtr TcpFlagsProcessor::CreateCopy() const
{
    return IProcessorPtr(new TcpFlagsProcessor(nextProcessor->CreateCopy()));
}

ProcessingStatus TcpFlagsProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(Self);
    tcp_header * tcp = (tcp_header *)(&packet->data()[0] + offset);
    
    Abonent & abonent =
        (packet->direction() == IPacket::ClientToServer)
        ? client : server;

    typedef tcp_header::flags_struct FLAGS;

    switch (connectionStatus)
    {
        case CLOSED:
            if (tcp->flags.haveFlags(FLAGS::SYN))
            {
                connectionStatus = SYN_FIRST;
                client.seqNumber = tcp->seq;
                server.ackNumber = tcp->seq;
                client.recvBufferAckNumber = tcp->seq;
                server.sendBufferSeqNumber = tcp->seq;
                client.windowSize = tcp->window_size;

                packet->setDirection(IPacket::ClientToServer);
                tcp->flags = FLAGS::SYN;
                if (prevProcessor != NULL)
                    prevProcessor->backwardProcess(Protocol::TCP, packet, offset);
            }
            break;
        case SYN_FIRST:
            if (tcp->flags.haveFlags(FLAGS::ACK))
            {
                connectionStatus = SYN_SECOND;
            }
            //break;
        case SYN_SECOND:
            if (tcp->flags.haveFlags(FLAGS::SYN))
            {
                connectionStatus = SYN_THIRD;
                server.seqNumber = tcp->seq;
                client.ackNumber = tcp->seq;
                server.recvBufferAckNumber = tcp->seq;
                client.sendBufferSeqNumber = tcp->seq;
                server.windowSize = tcp->window_size;

                packet->setDirection(IPacket::ServerToClient);
                tcp->flags = FLAGS::SYN + FLAGS::ACK;
                if (prevProcessor != NULL)
                    prevProcessor->backwardProcess(Protocol::TCP, packet, offset);
            }
            break;
        case SYN_THIRD:
            if (tcp->flags.haveFlags(FLAGS::ACK))
            {
                connectionStatus = ESTABLISHED;
                client.recvBuffer.reserve(server.windowSize);
                client.sendBuffer.reserve(Abonent::defaultWindowsSize);
                server.recvBuffer.reserve(client.windowSize);
                server.sendBuffer.reserve(Abonent::defaultWindowsSize);

                packet->setDirection(IPacket::ClientToServer);
                tcp->flags = FLAGS::ACK;
                if (prevProcessor != NULL)
                    prevProcessor->backwardProcess(Protocol::TCP, packet, offset);
            }
            break;
        case ESTABLISHED:
            if (tcp->flags.haveFlags(FLAGS::FIN + FLAGS::ACK))
            {
                packet->swapDirection();
                tcp->flags = FLAGS::ACK;
                if (prevProcessor != NULL)
                    prevProcessor->backwardProcess(Protocol::TCP, packet, offset);
            } else
            if (tcp->flags.haveFlags(FLAGS::FIN))
            {
                packet->swapDirection();
                tcp->flags = FLAGS::FIN + FLAGS::ACK;
                if (prevProcessor != NULL)
                    prevProcessor->backwardProcess(Protocol::TCP, packet, offset);
            } else
            if (tcp->flags.haveFlags(FLAGS::ACK))
            {
                int dataSize = packet->realSize() - offset - tcp->header_size();

                // Prepare answer packet
                IPacketPtr newPacket = packet->CreateCopy();
                newPacket->swapDirection();
                newPacket->data().resize(offset+tcp->header_size());
                newPacket->setRealSize(offset+tcp->header_size());
                tcp_header * newTcp = (tcp_header *)(&newPacket->data()[0] + offset);
                newTcp->flags = FLAGS::ACK;

                if (dataSize == 0)
                {
                    // It's only acknowledgment

                    if (abonent.sendBufferSeqNumber == tcp->ack)
                    {
                        // No packet from queue committed
                        abonent.waitAckNumbers.clear();
                        transferDataToAbonentFromSN(packet, abonent, abonent.sendBufferSeqNumber);
                        if (prevProcessor != NULL)
                            prevProcessor->backwardProcess(Protocol::TCP, newPacket, offset);
                        return ProcessingStatus::Accepted;
                    }
                    else
                    if (*abonent.waitAckNumbers.begin() == tcp->ack)
                    {
                        // First packet committed
                        abonent.waitAckNumbers.erase(abonent.waitAckNumbers.begin());
                        int commitSize = tcp->ack - abonent.sendBufferSeqNumber;
                        abonent.sendBufferSeqNumber = tcp->ack;
                        abonent.sendBuffer.erase(abonent.sendBuffer.begin(), abonent.sendBuffer.begin() + commitSize);
                        return ProcessingStatus::Accepted;
                    }
                    else
                    {
                        std::vector<u32be>::iterator it =
                            std::find(abonent.waitAckNumbers.begin(), abonent.waitAckNumbers.end(), tcp->ack);

                        if (it != abonent.waitAckNumbers.end())
                        {
                            // Any packet committed
                            abonent.waitAckNumbers.erase(it);
                        }
                        else
                        {
                            // Unexpected commit
                            return ProcessingStatus::Accepted;
                        }
                    }
                }
                else
                {
                    // It's packet with data
                    
                    int off = tcp->seq - abonent.recvBufferAckNumber;
                    std::copy(&packet->data()[offset + tcp->header_size()],
                              &packet->data()[0] + packet->realSize(),
                              abonent.recvBuffer.begin() + off);

                    if (tcp->flags.haveFlags(FLAGS::PSH))
                    {
                        // Push data to next processor
                        int infoSize = tcp->seq + dataSize - abonent.recvBufferAckNumber;
                        IPacketPtr dataPacket = packet->CreateCopy();
                        dataPacket->data().resize(offset+tcp->header_size()+infoSize);
                        dataPacket->setRealSize(offset+tcp->header_size()+infoSize);
                        std::copy(abonent.recvBuffer.begin(),
                                  abonent.recvBuffer.begin() + infoSize,
                                  &dataPacket->data()[offset + tcp->header_size()]);
                        abonent.recvBuffer.erase(
                            abonent.recvBuffer.begin(),
                            abonent.recvBuffer.begin() + infoSize);
                        abonent.recvBufferAckNumber += infoSize;
                        
                        if (nextProcessor != NULL)
                            nextProcessor->forwardProcess(proto, dataPacket, offset);
                    }

                    prevProcessor->backwardProcess(Protocol::TCP, newPacket, offset);
                }                
            }
            break;
    }

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpFlagsProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    tcp_header * tcp = (tcp_header *)(&packet->data()[0] + offset);
    memset(tcp, 0, sizeof(tcp_header));

    if (prevProcessor != NULL)
        prevProcessor->backwardProcess(Protocol::TCP, packet, offset);

    return ProcessingStatus::Accepted;
}

void TcpFlagsProcessor::transferDataToAbonentFromSN(IPacketPtr & packet, Abonent & abonent, u32be startSeqNumber)
{
    int off = startSeqNumber - abonent.sendBufferSeqNumber;
    int sz = std::min((size_t)1480, abonent.sendBuffer.size());
    packet->data().resize(packet->data().size() + sz);
    std::copy(abonent.sendBuffer.begin()+off,
              abonent.sendBuffer.begin()+off+sz,
              packet->data().end() - sz);
    abonent.waitAckNumbers.push_back(abonent.sendBufferSeqNumber+off+sz);
}

const char * TcpFlagsProcessor::getProcessorName()
{
    return "TcpFlagsProcessor";
}

Protocol TcpFlagsProcessor::getProtocol()
{
    return Protocol::TCP;
}
