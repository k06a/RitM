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

    if (abonent.initialSN == 0)
    {
        abonent.initialSN = tcp->seq;
        abonent.currentRecvSN = tcp->seq;
        if (toAbonent.currentSendSN == 0)
            toAbonent.currentSendSN = tcp->seq;
    }

    //                   <Seq>
    //   |--------|--------V--------|--------|
    // 1.    |--------|
    // 2.                  |
    // 3.            |--------|
    // 4.                      |--------|
    //

    // Empty packet or in range
    if (tcp->seq + dataInTcp < abonent.currentRecvSN)
    {
        // Skip old packet
        return ProcessingStatus::Accepted;            
    } else
    if ((tcp->seq + dataInTcp == abonent.currentRecvSN) && (dataInTcp == 0))
    {
        // Add to queue
    } else
    if ((tcp->seq < abonent.currentRecvSN) &&
        (tcp->seq + dataInTcp > abonent.currentRecvSN))
    {
        // Cut begin of the packet
        int sizeToRemove = abonent.currentRecvSN - tcp->seq;
        packet->data().erase(
            packet->data().begin() + offset,
            packet->data().begin() + offset + sizeToRemove);
        packet->setRealSize(packet->realSize() - sizeToRemove);
    } else
    if (tcp->seq > abonent.currentRecvSN)
    {
        // Add to queue
    }

    // Add to the end
    abonent.recvBuffer.push_back(
        AbonentSN::QuededPacket(tcp->seq, proto, packet, offset, dataInTcp));

    // Sort elements
    std::inplace_merge(
        abonent.recvBuffer.begin(),
        abonent.recvBuffer.end()-1,
        abonent.recvBuffer.end());

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
