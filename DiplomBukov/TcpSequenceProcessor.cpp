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

    tcp_header * tcp = (tcp_header *)&packet->data()[offset];
    int dataInTcp = packet->realSize() - offset - tcp->header_size();

    Abonent & abonent =
        (packet->direction() == IPacket::ClientToServer)
        ? client : server;

    Abonent & toAbonent =
        (packet->direction() == IPacket::ClientToServer)
        ? server : client;

    if (abonent.initialSN == 0)
    {
        abonent.initialSN = tcp->seq;
        abonent.currentRecvSN = tcp->seq+1;
        toAbonent.currentSendSN = tcp->seq+1;
    }

    abonent.currentRecvSN += dataInTcp;

    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpSequenceProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    tcp_header * tcp = (tcp_header *)&packet->data()[offset];
    int dataInTcp = packet->data().size() - offset - tcp->header_size();
    
    Abonent & abonent =
        (packet->direction() == IPacket::ClientToServer)
        ? client : server;

    Abonent & toAbonent =
        (packet->direction() == IPacket::ClientToServer)
        ? server : client;

    tcp->seq = toAbonent.currentSendSN;
    tcp->ack = toAbonent.currentRecvSN;
    
    if (packet->prevProcessor(Self) != NULL)
        packet->prevProcessor(Self)->backwardProcess(proto, packet, offset);

    toAbonent.currentSendSN += dataInTcp;
    tcp->seq = toAbonent.currentSendSN;
    
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