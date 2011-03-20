#include "TcpProtocolProcessor.h"
#include "tcp_header.h"
#include <algorithm>

using namespace DiplomBukov;

TcpProtocolProcessor::TcpProtocolProcessor(IProcessorPtr Connector)
    : maxDataInTcp(0)
{
    setNextProcessor(Connector);
}

IProcessorPtr TcpProtocolProcessor::CreateCopy() const
{
    IProcessorPtr ptr(new TcpProtocolProcessor(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus TcpProtocolProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    tcp_header * tcp = (tcp_header *)&packet->data()[offset];
    unsigned dataInTcp = packet->size() - offset - tcp->header_size();
    if (dataInTcp > maxDataInTcp)
        maxDataInTcp = dataInTcp;

    AbonentSN & abonent =
        (packet->direction() == IPacket::ClientToServer)
        ? client : server;

    AbonentSN & toAbonent =
        (packet->direction() == IPacket::ClientToServer)
        ? server : client;

    //////////////////////////////////////////////////////////////////////////

    if (abonent.initialSN == 0)
    {
        abonent.initialSN = tcp->seq;
        abonent.currentRecvSN = tcp->seq+1;
        toAbonent.currentSendSN = tcp->seq+1;
    }

    if (tcp->flags.haveFlags(tcp_header::flags_struct::SYN))
        return ProcessingStatus::Accepted;

    //////////////////////////////////////////////////////////////////////////

    if ((dataInTcp == 0) && (tcp->flags == tcp_header::flags_struct::ACK))
    {
        // Commit packet
        std::deque<AbonentSN::QuededPacket>::iterator it = 
            std::find(abonent.commitWaitQueue.begin(),
            abonent.commitWaitQueue.end(), tcp->ack);

        if (it != abonent.commitWaitQueue.end())
        {
            abonent.commitWaitQueue.erase(it);
            abonent.currentSendSN = tcp->ack;
        }

        if (abonent.toSendBuffer.size() > 0)
        {
            AbonentSN::QuededPacket & pack = abonent.toSendBuffer.front();
            privateBackwardProcess(pack.proto, pack.packet, pack.offset);
            abonent.toSendBuffer.pop_front();
        }

        return ProcessingStatus::Accepted;
    }

    if (dataInTcp != 0)
    {
        if (tcp->seq != abonent.currentRecvSN)
        {
            // Send last ack
            /*
            privateBackwardProcess(
                    abonent.lastAck.proto,
                    abonent.lastAck.packet,
                    abonent.lastAck.offset);
            */
        }
        else
        {
            packet->addProcessor(Self);
            abonent.currentRecvSN += dataInTcp;

            // Add to queue
            AbonentSN::QuededPacket qpacket = 
                AbonentSN::QuededPacket(tcp->seq, proto, packet, offset, dataInTcp);
            abonent.recvWaitQueue.push_back(qpacket);

            // Send ACK
            IPacketPtr ackpack = createAck(qpacket);
            privateBackwardProcess(proto, ackpack, offset);
            abonent.lastAck = AbonentSN::QuededPacket(tcp->seq, proto, ackpack, offset, 0);
            
            if (tcp->flags.haveFlags(tcp_header::flags_struct::PSH))
            {
                // End of message
                std::pair<IPacketPtr,unsigned> para = mergePackets(abonent.recvWaitQueue);
                IPacketPtr pack = para.first;
                unsigned offset = para.second;
                abonent.recvWaitQueue.clear();
                if (nextProcessor != NULL)
                    nextProcessor->forwardProcess(proto, pack, offset);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpProtocolProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    tcp_header * tcp = (tcp_header *)&packet->data()[offset];
    int dataInTcp = packet->data().size() - offset - tcp->header_size();

    AbonentSN & toAbonent =
        (packet->direction() == IPacket::ClientToServer)
        ? server : client;

    if (dataInTcp != 0)
    {
        for (int i = 0; true; i++)
        {
            IPacketPtr packetCopy = packet->CreateCopy();

            int a_delete_from = offset + tcp->header_size();
            int a_delete_to = a_delete_from + i*maxDataInTcp;
            int b_delete_from = a_delete_to + maxDataInTcp;
            int b_delete_to = packetCopy->size();

            if (b_delete_from > b_delete_to)
                b_delete_from = b_delete_to;

            packetCopy->data().erase(
                packetCopy->data().begin()+a_delete_from,
                packetCopy->data().begin()+a_delete_to);
            packetCopy->data().erase(
                packetCopy->data().begin()+b_delete_from,
                packetCopy->data().begin()+b_delete_to);

            toAbonent.toSendBuffer.push_back(
                AbonentSN::QuededPacket(0, proto, packetCopy, offset,
                                        b_delete_from-a_delete_to));

            if (b_delete_from == b_delete_to)
                break;
        }
    }

    AbonentSN::QuededPacket & qp = toAbonent.toSendBuffer.front();
    privateBackwardProcess(qp.proto, qp.packet, qp.offset);
    toAbonent.toSendBuffer.pop_front();

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpProtocolProcessor::privateBackwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    tcp_header * tcp = (tcp_header *)&packet->data()[offset];
    int dataInTcp = packet->data().size() - offset - tcp->header_size();

    AbonentSN & toAbonent =
        (packet->direction() == IPacket::ClientToServer)
        ? server : client;

    tcp->seq = toAbonent.currentSendSN;
    tcp->ack = toAbonent.currentRecvSN;
    
    if (packet->prevProcessor(Self) != NULL)
        packet->prevProcessor(Self)->backwardProcess(proto, packet, offset);

    toAbonent.commitWaitQueue.push_back(
        AbonentSN::QuededPacket(tcp->seq+dataInTcp, proto, packet, offset, dataInTcp));

    return ProcessingStatus::Accepted;
}

const char * TcpProtocolProcessor::getProcessorName()
{
    return "TcpProtocolProcessor";
}

Protocol TcpProtocolProcessor::getProtocol()
{
    return Protocol::TCP;
}

IPacketPtr TcpProtocolProcessor::createAck(const AbonentSN::QuededPacket & qpacket)
{
    IPacketPtr pack = qpacket.packet->CreateCopy();
    int dataInTcp = qpacket.dataInTcp;

    pack->swapDirection();
    pack->setRealSize(pack->realSize() - dataInTcp);
    pack->data().resize(pack->realSize());

    AbonentSN & toAbonent =
        (pack->direction() == IPacket::ClientToServer)
        ? server : client;

    tcp_header * tcp = (tcp_header *)&pack->data()[qpacket.offset];
    tcp->flags = tcp_header::flags_struct::ACK;
    tcp->seq = toAbonent.currentSendSN;
    tcp->ack = toAbonent.currentRecvSN;

    return pack;
}

std::pair<IPacketPtr,unsigned> TcpProtocolProcessor::mergePackets(const std::deque<AbonentSN::QuededPacket> & arr)
{
    IPacketPtr pack = arr[0].packet->CreateCopy();
    pack->data().resize(arr[0].offset);
    
    for (std::deque<AbonentSN::QuededPacket>::const_iterator it = arr.begin();
        it != arr.end(); ++it)
    {
        // ¬ставка в конец сырых данных из пакета
        pack->data().insert(
            pack->data().end(),
            it->packet->data().begin()+it->offset,
            it->packet->data().end());
    }

    pack->setRealSize(pack->data().size());
    return std::make_pair(pack, arr[0].offset);
}