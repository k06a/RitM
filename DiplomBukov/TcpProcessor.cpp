#include "TcpProcessor.h"
#include "tcp_header.h"

using namespace DiplomBukov;

TcpProcessor::TcpProcessor(IProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

IProcessorPtr TcpProcessor::CreateCopy() const
{
    return IProcessorPtr(new TcpProcessor(nextProcessor->CreateCopy()));
}

ProcessingStatus TcpProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(Self);

    tcp_header * tcp = (tcp_header *)(packet->data() + offset);

    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(Protocol::None, packet, offset + tcp->header_size());

    return ProcessingStatus::Accepted;
}

const char * TcpProcessor::getProcessorName()
{
    return "TcpProcessor";
}

Protocol TcpProcessor::getProtocol()
{
    return Protocol::TCP;
}
