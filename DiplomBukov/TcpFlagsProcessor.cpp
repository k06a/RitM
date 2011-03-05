#include "TcpFlagsProcessor.h"

using namespace DiplomBukov;

TcpFlagsProcessor::TcpFlagsProcessor(IProcessorPtr Connector)
    : connectionStatus(CLOSED)
    , flagsForClient()
    , flagsForServer()
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
    
    tcp_header::flags_struct & expectedFlags =
        (packet->direction() == IPacket::ClientToServer)
        ? flagsForClient : flagsForServer;

    typedef tcp_header::flags_struct FLAGS;

    switch (connectionStatus)
    {
        case CLOSED:
            if (tcp->flags >= FLAGS::SYN)
            {
                connectionStatus = SYN_FIRST;
                
                packet->setDirection(IPacket::ClientToServer);
                tcp->flags = FLAGS::SYN;
                if (prevProcessor != NULL)
                    prevProcessor->backwardProcess(Protocol::TCP, packet, offset);
            }
            break;
        case SYN_FIRST:
            if (tcp->flags >= FLAGS::SYN + FLAGS::ACK)
            {
                connectionStatus = SYN_SECOND;

                packet->setDirection(IPacket::ServerToClient);
                tcp->flags = FLAGS::SYN + FLAGS::ACK;
                if (prevProcessor != NULL)
                    prevProcessor->backwardProcess(Protocol::TCP, packet, offset);
            }
            break;
        case SYN_SECOND:
            if (tcp->flags >= FLAGS::ACK)
            {
                connectionStatus = ESTABLISHED;

                packet->setDirection(IPacket::ClientToServer);
                tcp->flags = FLAGS::ACK;
                if (prevProcessor != NULL)
                    prevProcessor->backwardProcess(Protocol::TCP, packet, offset);
            }
            break;
        case ESTABLISHED:
            if (tcp->flags >= FLAGS::ACK)
            {
                // Commit

                IPacketPtr newPacket = packet->CreateCopy();
                newPacket->swapDirection();
                newPacket->data().resize(offset+tcp->header_size());
                newPacket->setRealSize(offset+tcp->header_size());

                tcp_header * newTcp = (tcp_header *)(&newPacket->data()[0] + offset);
                tcp->flags = FLAGS::ACK;
                if (prevProcessor != NULL)
                    prevProcessor->backwardProcess(Protocol::TCP, newPacket, offset);

                // Send data to next processor

                if (nextProcessor != NULL)
                    nextProcessor->forwardProcess(proto, packet, offset);
            } else
            if (tcp->flags >= FLAGS::FIN + FLAGS::ACK)
            {
                packet->swapDirection();
                tcp->flags = FLAGS::ACK;
                if (prevProcessor != NULL)
                    prevProcessor->backwardProcess(Protocol::TCP, packet, offset);
            } else
            if (tcp->flags >= FLAGS::FIN)
            {
                packet->swapDirection();
                tcp->flags = FLAGS::FIN + FLAGS::ACK;
                if (prevProcessor != NULL)
                    prevProcessor->backwardProcess(Protocol::TCP, packet, offset);
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

const char * TcpFlagsProcessor::getProcessorName()
{
    return "TcpFlagsProcessor";
}

Protocol TcpFlagsProcessor::getProtocol()
{
    return Protocol::TCP;
}
