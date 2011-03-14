#include "TcpSequenceProcessor.h"
#include "tcp_header.h"
#include <algorithm>

using namespace DiplomBukov;

TcpSequenceProcessor::TcpSequenceProcessor(IProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

IProcessorPtr TcpSequenceProcessor::CreateCopy() const
{
    IProcessorPtr ptr(new TcpSequenceProcessor(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus TcpSequenceProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    tcp_header * tcp = (tcp_header *)(&packet->data()[0] + offset);
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
        abonent.currentRecvSN = tcp->seq;
        if (toAbonent.currentSendSN == 0)
            toAbonent.currentSendSN = tcp->seq;
    }

    //////////////////////////////////////////////////////////////////////////

    int situation = 0;
    situation += OldPacket*(tcp->seq + dataInTcp < abonent.currentRecvSN);
    situation += ExpectedPacket*(tcp->seq == abonent.currentRecvSN);
    situation += CutPacket*((tcp->seq < abonent.currentRecvSN) && (tcp->seq + dataInTcp > abonent.currentRecvSN));
    situation += FuturePacket*(tcp->seq > abonent.currentRecvSN);
    if (situation == 0)
        situation += CommitPacket*(dataInTcp == 0);

    bool shouldSave = true;
    bool shouldProcess = false;
    switch (situation)
    {
        case OldPacket:
            return ProcessingStatus::Accepted; 
            
        case ExpectedPacket:
            break;

        case CutPacket:
            {
                int sizeToRemove = abonent.currentRecvSN - tcp->seq;
                packet->data().erase(
                    packet->data().begin() + offset,
                    packet->data().begin() + offset + sizeToRemove);
                packet->setRealSize(packet->realSize() - sizeToRemove);
                tcp->seq += sizeToRemove;
                situation = ExpectedPacket;
            } break;

        case FuturePacket:
            shouldProcess = true;
            break;

        case CommitPacket:
            shouldProcess = true;
            shouldSave = false;
            break;

        default:
            throw "Unknown situation";
    }

    //////////////////////////////////////////////////////////////////////////

    AbonentSN::QuededPacket qpack(tcp->seq, proto, packet, offset, dataInTcp);

    if (shouldProcess)
        forwardProcess2(&qpack, (PacketSituation)situation);

    if (shouldSave)
    {
        // Add to the end
        abonent.recvWaitQueue.push_back(qpack);
        
        // Sort elements
        std::inplace_merge(
            abonent.recvWaitQueue.begin(),
            abonent.recvWaitQueue.end()-1,
            abonent.recvWaitQueue.end());
    }

    //////////////////////////////////////////////////////////////////////////

    if (abonent.recvWaitQueue.size() == 0)
        return ProcessingStatus::Accepted;

    AbonentSN::QuededPacket * first = &abonent.recvWaitQueue.front();
    tcp_header * tcp_h = (tcp_header*)&first->packet->data()[first->offset];
    while (tcp_h->seq == abonent.currentRecvSN)
    {
        first->packet->addProcessor(Self);
        forwardProcess2(first, ExpectedPacket);

        abonent.currentRecvSN += first->dataInTcp;
        abonent.recvWaitQueue.pop_front();
        if (abonent.recvWaitQueue.size() == 0)
            break;
        first = &abonent.recvWaitQueue.front();
    }

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpSequenceProcessor::forwardProcess2(AbonentSN::QuededPacket * qpacket, PacketSituation situation)
{
    AbonentSN & abonent =
        (qpacket->packet->direction() == IPacket::ClientToServer)
        ? client : server;

    AbonentSN & toAbonent =
        (qpacket->packet->direction() == IPacket::ClientToServer)
        ? server : client;

    if (situation == CommitPacket)
    {
        tcp_header * tcp = (tcp_header *)&qpacket->packet->data()[qpacket->offset];

        // ѕоиск и удаление пакета с подтвержденным номером
        for(std::deque<AbonentSN::QuededPacket>::const_iterator it =
            abonent.sendWaitQueue.begin(); it != abonent.sendWaitQueue.end(); ++it)
        {
            tcp_header * tcp_h = (tcp_header *)&it->packet->data()[it->offset];
            if (tcp->seq == tcp_h->seq)
            {
                abonent.sendWaitQueue.erase(it);
                break;
            }
        }
        
        return ProcessingStatus::Accepted;
    }

    Protocol proto = qpacket->proto;
    unsigned offset = qpacket->offset;
    IPacketPtr ackpack = createAck(qpacket, abonent);

    if (ackpack->prevProcessor(Self) != NULL)
        ackpack->prevProcessor(Self)->backwardProcess(proto, ackpack, offset);

    if (situation == ExpectedPacket)
    {
        abonent.toRecvBuffer.push_back(*qpacket);
        tcp_header * tcp = (tcp_header *)(&qpacket->packet->data()[0] + qpacket->offset);
        if (tcp->flags.haveFlags(tcp_header::flags_struct::PSH))
        {
            std::pair<IPacketPtr,unsigned> para = mergePackets(abonent.toRecvBuffer);
            IPacketPtr pack = para.first;
            unsigned offset = para.second;
            abonent.toRecvBuffer.clear();
            if (nextProcessor != NULL)
                nextProcessor->forwardProcess(proto, pack, offset);
        }
    }

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpSequenceProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    tcp_header * tcp = (tcp_header *)(&packet->data()[0] + offset);
    int dataInTcp = packet->size() - offset - tcp->header_size();
    
    AbonentSN & abonent =
        (packet->direction() == IPacket::ClientToServer)
        ? client : server;

    tcp->seq = abonent.currentSendSN;
    tcp->ack = abonent.currentRecvSN;
    abonent.currentSendSN += dataInTcp;
    
    if (packet->prevProcessor(Self) != NULL)
        packet->prevProcessor(Self)->backwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

const char * TcpSequenceProcessor::getProcessorName()
{
    return "TcpSequenceProcessor";
}

Protocol TcpSequenceProcessor::getProtocol()
{
    return Protocol::TCP;
}

IPacketPtr TcpSequenceProcessor::createAck(AbonentSN::QuededPacket * qpacket, AbonentSN & abonent)
{
    IPacketPtr pack = qpacket->packet->CreateCopy();
    int dataInTcp = qpacket->dataInTcp;

    pack->setDirection(
        (pack->direction() == IPacket::ClientToServer)
        ? IPacket::ServerToClient
        : IPacket::ClientToServer);
    pack->setRealSize(pack->realSize() - dataInTcp);
    pack->data().resize(pack->realSize());

    tcp_header * tcp = (tcp_header *)&pack->data()[qpacket->offset];
    tcp->seq = abonent.currentSendSN;
    tcp->ack = abonent.currentRecvSN;

    return pack;
}

std::pair<IPacketPtr,unsigned> TcpSequenceProcessor::mergePackets(const std::deque<AbonentSN::QuededPacket> & arr)
{
    IPacketPtr pack = arr[0].packet->CreateCopy();
    unsigned offset = arr[0].offset;
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
    return std::make_pair(pack,offset);
}