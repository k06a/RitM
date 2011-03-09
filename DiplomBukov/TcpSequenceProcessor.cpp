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

    //                   <Seq>
    //   |--------|--------V--------|--------|
    // 1.    |--------|                         // LastPacket
    // 2.                  |                    // ExpectedPacket
    // 4.            |--------|                 // CutPacket
    // 8.                      |--------|       // FuturePacket
    //

    enum PacketSituation
    {
        LastPacket     = 1, // Пакет из прошлого
        ExpectedPacket = 2, // Пакет с ожидаемым номером
        CutPacket      = 4, // Пакет с частью новых данных
        FuturePacket   = 8  // Пакет из будущего
    };

    int situation = 0;
    situation += LastPacket*(tcp->seq + dataInTcp < abonent.currentRecvSN);
    situation += ExpectedPacket*((tcp->seq + dataInTcp == abonent.currentRecvSN) && (dataInTcp == 0));
    situation += CutPacket*((tcp->seq < abonent.currentRecvSN) && (tcp->seq + dataInTcp > abonent.currentRecvSN));
    situation += FuturePacket*(tcp->seq > abonent.currentRecvSN);

    switch (situation)
    {
        case LastPacket:
            return ProcessingStatus::Accepted; 
            
        case ExpectedPacket:
            break;

        case CutPacket: {
            int sizeToRemove = abonent.currentRecvSN - tcp->seq;
            packet->data().erase(
                packet->data().begin() + offset,
                packet->data().begin() + offset + sizeToRemove);
            packet->setRealSize(packet->realSize() - sizeToRemove);
            break; }

        case FuturePacket:
            break;

        default:
            throw "Unknown situation";
    }

    //////////////////////////////////////////////////////////////////////////

    // Add to the end
    abonent.recvBuffer.push_back(
        AbonentSN::QuededPacket(tcp->seq, proto, packet, offset, dataInTcp));

    // Sort elements
    std::inplace_merge(
        abonent.recvBuffer.begin(),
        abonent.recvBuffer.end()-1,
        abonent.recvBuffer.end());

    //////////////////////////////////////////////////////////////////////////

    AbonentSN::QuededPacket * first = &abonent.recvBuffer.front();
    while (first->offset == abonent.currentRecvSN)
    {
        first->packet->addProcessor(Self);
        if (nextProcessor != NULL)
            nextProcessor->forwardProcess(first->proto, first->packet, first->offset);

        abonent.currentRecvSN += first->dataInTcp;
        abonent.recvBuffer.pop_front();
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
