#include "TcpProtocolProcessor.h"
#include "tcp_header.h"
#include <algorithm>

using namespace DiplomBukov;

TcpProtocolProcessor::TcpProtocolProcessor(IProcessorPtr Connector)
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
    int dataInTcp = packet->realSize() - offset - tcp->header_size();

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
            std::find(abonent.sendWaitQueue.begin(),
            abonent.sendWaitQueue.end(), tcp->ack);

        if (it != abonent.sendWaitQueue.end())
            abonent.sendWaitQueue.erase(it);

        return ProcessingStatus::Accepted;
    }

    if (dataInTcp != 0)
    {
        if (tcp->seq != abonent.currentRecvSN)
        {
            // Send last ack
            /*
            if (abonent.lastAck.packet->prevProcessor(Self) != NULL)
                abonent.lastAck.packet->prevProcessor(Self)->backwardProcess(
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
            //IPacketPtr ackpack = createAck(abonent.currentSendSN, abonent.currentRecvSN, packet, dataInTcp, offset, abonent);
            IPacketPtr ackpack = createAck(&qpacket);
            backwardProcess(proto, ackpack, offset);
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
    
    /*AbonentSN & abonent =
        (packet->direction() == IPacket::ClientToServer)
        ? client : server;*/

    AbonentSN & toAbonent =
        (packet->direction() == IPacket::ClientToServer)
        ? server : client;

    tcp->seq = toAbonent.currentSendSN;
    tcp->ack = toAbonent.currentRecvSN;
    
    if (packet->prevProcessor(Self) != NULL)
        packet->prevProcessor(Self)->backwardProcess(proto, packet, offset);

    toAbonent.currentSendSN += dataInTcp;
    tcp->seq = toAbonent.currentSendSN;

    if (dataInTcp != 0)
        toAbonent.sendWaitQueue.push_back(
            AbonentSN::QuededPacket(tcp->seq, proto, packet, offset, dataInTcp));
    
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

IPacketPtr TcpProtocolProcessor::createAck(u32be seq, u32be ack, IPacketPtr packet, int dataInTcp, unsigned offset, AbonentSN & abonent)
{
    IPacketPtr pack = packet->CreateCopy();

    pack->swapDirection();
    pack->setRealSize(pack->realSize() - dataInTcp);
    pack->data().resize(pack->realSize());

    tcp_header * tcp = (tcp_header *)&pack->data()[offset];
    tcp->flags = tcp_header::flags_struct::ACK;
    tcp->seq = seq;
    tcp->ack = ack;

    return pack;
}

IPacketPtr TcpProtocolProcessor::createAck(AbonentSN::QuededPacket * qpacket)
{
    IPacketPtr pack = qpacket->packet->CreateCopy();
    int dataInTcp = qpacket->dataInTcp;

    pack->swapDirection();
    pack->setRealSize(pack->realSize() - dataInTcp);
    pack->data().resize(pack->realSize());

    AbonentSN & toAbonent =
        (pack->direction() == IPacket::ClientToServer)
        ? server : client;

    tcp_header * tcp = (tcp_header *)&pack->data()[qpacket->offset];
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