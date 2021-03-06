#include "TcpLayerProcessor.h"
#include "network/tcp_header.h"
#include <algorithm>

using namespace RitM;

TcpLayerProcessor::TcpLayerProcessor(ProcessorPtr Connector)
    : maxDataInTcp(0)
{
    setNextProcessor(Connector);
}

ProcessorPtr TcpLayerProcessor::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new TcpLayerProcessor(np));
}

void TcpLayerProcessor::DestroyHierarchy()
{
    client.commitWaitQueue.clear();
    client.recvWaitQueue.clear();
    client.toSendBuffer.clear();
    client.lastAck.packet = PacketPtr();
    server.commitWaitQueue.clear();
    server.recvWaitQueue.clear();
    server.toSendBuffer.clear();
    server.lastAck.packet = PacketPtr();
    AbstractProcessor::DestroyHierarchy();
}

ProcessingStatus TcpLayerProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(shared_from_this());
    tcp_header * tcp = (tcp_header *)&(*packet)[offset];
    unsigned dataInTcp = packet->size() - offset - tcp->header_size();
    maxDataInTcp = std::max(maxDataInTcp, dataInTcp);

    typedef tcp_header::flags_struct FLAGS;

    Abonent & abonent =
        (packet->direction() == IPacket::ClientToServer)
        ? client : server;

    Abonent & toAbonent =
        (packet->direction() == IPacket::ClientToServer)
        ? server : client;

    //////////////////////////////////////////////////////////////////////////

    switch (abonent.status)
    {
        case CLOSED:
            if (tcp->flags.haveFlags(FLAGS::SYN))
            {
                abonent.status = FIRST_SYN;
                toAbonent.status = FIRST_SYN;
                abonent.initialSN = tcp->seq;
                abonent.currentRecvSN = tcp->seq + 1;
                toAbonent.currentSendSN = tcp->seq;
                privateBackwardProcess(proto, packet, offset);
            }
            break;

        case FIRST_SYN:
            if (tcp->flags.haveFlags(FLAGS::SYN + FLAGS::ACK))
            {
                abonent.status = SECOND_SYNACK;
                toAbonent.status = SECOND_SYNACK;
                abonent.initialSN = tcp->seq;
                abonent.currentRecvSN = tcp->seq + 1;
                toAbonent.currentSendSN = tcp->seq;
                privateBackwardProcess(proto, packet, offset);
            }
            break;

        case SECOND_SYNACK:
            if (tcp->flags.haveFlags(FLAGS::ACK))
            {
                abonent.status = ESTABLISHED;
                toAbonent.status = ESTABLISHED;
                privateBackwardProcess(proto, packet, offset);
            }
            break;

        case ESTABLISHED:
            if (tcp->flags.haveFlags(FLAGS::FIN))
            {
                abonent.status = LAST_FINACK;
                toAbonent.shouldCloseConnection = true;
                abonent.currentRecvSN++;

                if ((toAbonent.recvWaitQueue.size() == 0) &&
                    (toAbonent.toSendBuffer.size() == 0))
                {
                    toAbonent.status = PRELAST_FIN;
                    privateBackwardProcess(proto, packet, offset);
                }

                // Answer to sender
                tcp->flags = FLAGS::FIN + FLAGS::ACK;
                packet->swapDirection();
                privateBackwardProcess(proto, packet, offset);
                break;
            }

            if (tcp->flags.haveFlags(FLAGS::ACK))
            {
                enum CommitType
                {
                    OldCommit     = 1,
                    GoodCommit    = 2,
                    FutureCommit  = 4,
                };

                enum MessageType
                {
                    OldMessage    = 1,
                    GoodMessage   = 2,
                    FutureMessage = 4
                };

                int commitType = 0;
                commitType |= OldCommit    * (tcp->ack <  abonent.currentSendSN);
                commitType |= GoodCommit   * (tcp->ack == abonent.currentSendSN);
                commitType |= FutureCommit * (tcp->ack >  abonent.currentSendSN);

                int messageType = 0;
                if (dataInTcp != 0)
                {
                    messageType |= OldMessage    * (tcp->seq <  abonent.currentRecvSN);
                    messageType |= GoodMessage   * (tcp->seq == abonent.currentRecvSN);
                    messageType |= FutureMessage * (tcp->seq >  abonent.currentRecvSN);
                }
                
                switch (commitType)
                {
                    case OldCommit:
                        {
                            if ((abonent.commitWaitQueue.size() != 0) &&
                                (dataInTcp == 0))
                            {
                                QuededPacket & qp = abonent.commitWaitQueue.front();
                                privateBackwardProcess(qp.proto, qp.packet, qp.offset);
                            }
                        }
                        break;
                    case GoodCommit:
                        {
                            // ������� ������� �������� �������������
                            if (abonent.commitWaitQueue.size() != 0)
                                abonent.commitWaitQueue.pop_front();

                            // ���� ���� ��� ����������
                            if (abonent.toSendBuffer.size() != 0)
                            {
                                QuededPacket & qp = abonent.toSendBuffer.front();
                                privateBackwardProcess(qp.proto, qp.packet, qp.offset);
                                abonent.commitWaitQueue.push_back(qp);
                                abonent.toSendBuffer.pop_front();
                            }

                            // ���� ���� ������� ����� � ������ ������ ����������
                            if ((abonent.shouldCloseConnection) &&
                                (abonent.recvWaitQueue.size() == 0) &&
                                (abonent.toSendBuffer.size() == 0))
                            {
                                abonent.status = PRELAST_FIN;
                                tcp->flags = FLAGS::FIN + FLAGS::ACK;
                                PacketPtr newPacket = packet->CreateCopy();
                                newPacket->swapDirection();
                                privateBackwardProcess(proto, newPacket, offset);
                            }
                        }
                        break;
                    case FutureCommit:
                        {
                            // Wow)))
                            int a = 0;
                            a = 1;
                        }
                        break;
                }

                // ------------------------------------------------

                switch(messageType)
                {
                    case OldMessage:
                        {
                            QuededPacket & qp = abonent.lastAck;
                            if (qp.packet != NULL)
                                privateBackwardProcess(qp.proto, qp.packet, qp.offset);
                        }
                        break;
                    case GoodMessage:
                        {
                            abonent.currentRecvSN += dataInTcp;

                            // Add to queue
                            QuededPacket qpacket = 
                                QuededPacket(tcp->seq, proto, packet, offset, dataInTcp);
                            abonent.recvWaitQueue.push_back(qpacket);

                            // Send ACK
                            PacketPtr ackpack = createAck(qpacket);
                            privateBackwardProcess(proto, ackpack, offset);
                            abonent.lastAck = QuededPacket(tcp->seq, proto, ackpack, offset, 0);

                            if (tcp->flags.haveFlags(tcp_header::flags_struct::PSH))
                            {
                                // End of message
                                std::pair<PacketPtr,unsigned> para = mergePackets(abonent.recvWaitQueue);
                                PacketPtr pack = para.first;
                                unsigned offset = para.second;
                                abonent.recvWaitQueue.clear();
                                if (nextProcessor != NULL)
                                    nextProcessor->forwardProcess(proto, pack, offset);
                            }
                        }
                        break;
                    case FutureMessage:
                        {
                            QuededPacket & qp = abonent.lastAck;
                            if (qp.packet != NULL)
                                privateBackwardProcess(qp.proto, qp.packet, qp.offset);
                        }
                        break;
                }
            }
            break;

        case PRELAST_FIN:
            {
                if (tcp->flags.haveFlags(FLAGS::FIN + FLAGS::ACK))
                {
                    abonent.status = CLOSED;
                    abonent.currentRecvSN++;
                    tcp->flags = FLAGS::ACK;
                    packet->swapDirection();
                    privateBackwardProcess(proto, packet, offset);
                }
            }
            break;

        case LAST_FINACK:
            {
                if (tcp->flags.haveFlags(FLAGS::ACK))
                {
                    abonent.status = CLOSED;
                    //privateBackwardProcess(proto, packet, offset);
                }
            }
            break;
    }

    packet->setStatus(IPacket::Rejected);
    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpLayerProcessor::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    tcp_header * tcp = (tcp_header *)&(*packet)[offset];
    int dataInTcp = packet->size() - offset - tcp->header_size();

    Abonent & toAbonent =
        (packet->direction() == IPacket::ClientToServer)
        ? server : client;

    if (dataInTcp != 0)
    {
        for (int i = 0; true; i++)
        {
            PacketPtr packetCopy = packet->CreateCopy();

            int a_delete_from = offset + tcp->header_size();
            int a_delete_to = a_delete_from + i*maxDataInTcp;
            int b_delete_from = a_delete_to + maxDataInTcp;
            int b_delete_to = packetCopy->size();

            if (b_delete_from > b_delete_to)
                b_delete_from = b_delete_to;

            packetCopy->erase(
                b_delete_from,
                b_delete_to);
            packetCopy->erase(
                a_delete_from,
                a_delete_to);

            toAbonent.toSendBuffer.push_back(
                QuededPacket(0, proto, packetCopy, offset,
                                        b_delete_from-a_delete_to));

            if (b_delete_from == b_delete_to)
                break;
        }
    }

    QuededPacket & qp = toAbonent.toSendBuffer.front();
    privateBackwardProcess(qp.proto, qp.packet, qp.offset);
    toAbonent.toSendBuffer.pop_front();

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpLayerProcessor::privateBackwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    tcp_header * tcp = (tcp_header *)&(*packet)[offset];
    int dataInTcp = packet->size() - offset - tcp->header_size();

    Abonent & toAbonent =
        (packet->direction() == IPacket::ClientToServer)
        ? server : client;

    tcp->seq = toAbonent.currentSendSN;
    tcp->ack = toAbonent.currentRecvSN;
    
    if (packet->processorBefore(shared_from_this()) != NULL)
        packet->processorBefore(shared_from_this())->backwardProcess(proto, packet, offset);

    if (tcp->flags.haveFlags(tcp_header::flags_struct::SYN))
        toAbonent.currentSendSN++;
    if (tcp->flags.haveFlags(tcp_header::flags_struct::FIN))
        toAbonent.currentSendSN++;
    toAbonent.currentSendSN += dataInTcp;
    tcp->seq = toAbonent.currentSendSN;

    if (dataInTcp != 0)
        toAbonent.commitWaitQueue.push_back(
            QuededPacket(tcp->seq, proto, packet, offset, dataInTcp));

    return ProcessingStatus::Accepted;
}

const char * TcpLayerProcessor::getProcessorName()
{
    return "TcpLayerProcessor";
}

Protocol TcpLayerProcessor::getProtocol()
{
    return Protocol::TCP;
}

PacketPtr TcpLayerProcessor::createAck(const QuededPacket & qpacket)
{
    PacketPtr pack = qpacket.packet->CreateCopy();
    int dataInTcp = qpacket.dataInTcp;

    pack->swapDirection();
    pack->setRealSize(pack->realSize() - dataInTcp);
    pack->resize(pack->realSize());

    Abonent & abonent =
        (pack->direction() == IPacket::ClientToServer)
        ? client : server;

    Abonent & toAbonent =
        (pack->direction() == IPacket::ClientToServer)
        ? server : client;

    tcp_header * tcp = (tcp_header *)&(*pack)[qpacket.offset];
    tcp->flags = tcp_header::flags_struct::ACK;
    tcp->seq = toAbonent.currentSendSN;
    tcp->ack = toAbonent.currentRecvSN;

    return pack;
}

std::pair<PacketPtr,unsigned> TcpLayerProcessor::mergePackets(const std::deque<QuededPacket> & arr)
{
    PacketPtr pack = arr[0].packet->CreateCopy();
    pack->resize(arr[0].offset);
    
    for (std::deque<QuededPacket>::const_iterator it = arr.begin();
        it != arr.end(); ++it)
    {
        // ������� � ����� ����� ������ �� ������
        pack->insert(
            pack->size(),
            &(*it->packet)[it->offset],
            it->packet->size() - it->offset);
    }

    pack->setRealSize(pack->size());
    return std::make_pair(pack, arr[0].offset);
}