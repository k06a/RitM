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
    return IProcessorPtr(new TcpSequenceProcessor(nextProcessor->CreateCopy()));
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
    situation += ExpectedPacket*((tcp->seq + dataInTcp == abonent.currentRecvSN) && (dataInTcp == 0));
    situation += CutPacket*((tcp->seq < abonent.currentRecvSN) && (tcp->seq + dataInTcp > abonent.currentRecvSN));
    situation += FuturePacket*(tcp->seq > abonent.currentRecvSN);

    bool shouldProcess = false;
    switch (situation)
    {
        case OldPacket:
            return ProcessingStatus::Accepted; 
            
        case ExpectedPacket:
            break;

        case CutPacket: {
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

        default:
            throw "Unknown situation";
    }

    //////////////////////////////////////////////////////////////////////////

    // Add to the end
    abonent.recvWaitQueue.push_back(
        AbonentSN::QuededPacket(tcp->seq, proto, packet, offset, dataInTcp));

    if (shouldProcess)
        forwardProcess2(&abonent.recvWaitQueue.back(), (PacketSituation)situation);

    // Sort elements
    std::inplace_merge(
        abonent.recvWaitQueue.begin(),
        abonent.recvWaitQueue.end()-1,
        abonent.recvWaitQueue.end());

    //////////////////////////////////////////////////////////////////////////

    AbonentSN::QuededPacket * first = &abonent.recvWaitQueue.front();
    while (first->offset == abonent.currentRecvSN)
    {
        first->packet->addProcessor(Self);
        forwardProcess2(first, (PacketSituation)situation);

        abonent.currentRecvSN += first->dataInTcp;
        abonent.recvWaitQueue.pop_front();
    }

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpSequenceProcessor::forwardProcess2(AbonentSN::QuededPacket * packet, PacketSituation situation)
{
    AbonentSN & abonent =
        (packet->packet->direction() == IPacket::ClientToServer)
        ? client : server;

    AbonentSN & toAbonent =
        (packet->packet->direction() == IPacket::ClientToServer)
        ? server : client;

    switch (situation)
    {
        case OldPacket:
            return ProcessingStatus::Accepted; 

        case ExpectedPacket:
            abonent.toRecvBuffer.push_back(*packet);

        case FuturePacket: {
            Protocol proto = packet->proto;
            IPacketPtr pack = packet->packet->CreateCopy();
            unsigned offset = packet->offset;
            int dataInTcp = packet->dataInTcp;
            
            pack->setDirection(
                (pack->direction() == IPacket::ClientToServer)
                ? IPacket::ServerToClient
                : IPacket::ClientToServer);
            pack->setRealSize(pack->realSize() - dataInTcp);
            pack->data().resize(pack->realSize());
            tcp_header * tcp = (tcp_header *)(&pack->data()[0] + offset);
            tcp->seq = abonent.currentSendSN;
            tcp->ack = abonent.currentRecvSN;
            if (prevProcessor != NULL)
                prevProcessor->backwardProcess(proto, pack, offset);
            } break;

        default:
            throw "Unknown situation";
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
    
    if (prevProcessor != NULL)
        prevProcessor->backwardProcess(proto, packet, offset);

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
