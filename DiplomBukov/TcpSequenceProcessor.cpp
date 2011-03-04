#include "TcpSequenceProcessor.h"
#include "tcp_header.h"

using namespace DiplomBukov;

TcpSequenceProcessor::TcpSequenceProcessor(IProcessorPtr Connector)
    : originClientSN(0)
    , originServerSN(0)
    , actualClientSN(0)
    , actualServerSN(0)
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
    int dataInTcp = packet->size() - offset - tcp->header_size();

    // First packet - get client SEQ
    if (originClientSN == 0)
    {
        originClientSN = tcp->seq;
        actualClientSN = tcp->seq;
    }

    // Second packet - get server SEQ
    if ((originServerSN == 0) && (originClientSN != 0))
    {
        originServerSN = tcp->seq;
        actualServerSN = tcp->seq;
    }

    // Aliasing
    u32be & actualSn =
        (packet->direction() == IPacket::ClientToServer)
        ? actualClientSN : actualServerSN;

    if (tcp->seq != actualSn)
    {
        // Invalid sequence number
        //
        return ProcessingStatus::Accepted;
    }

    actualSn += dataInTcp;

    packet->addProcessor(Self);
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpSequenceProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    tcp_header * tcp = (tcp_header *)(&packet->data()[0] + offset);
    int dataInTcp = packet->size() - offset - tcp->header_size();
    
    if (packet->direction() == IPacket::ServerToClient)
    {
        actualServerSN += dataInTcp;
        tcp->seq = actualServerSN;
        tcp->ack = actualClientSN;
    }
    else
    {
        actualClientSN += dataInTcp;
        tcp->seq = actualClientSN;
        tcp->ack = actualServerSN;
    }

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
