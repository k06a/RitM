#include "UdpSplitter.h"

using namespace RitM;

UdpSplitter::UdpSplitter(ProcessorPtr Connector)
    : TcpSplitter(Connector)
{
}

ProcessorPtr UdpSplitter::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new UdpSplitter(np));
}

Protocol UdpSplitter::getProtocol()
{
    return Protocol::UDP;
}

const char * UdpSplitter::getProcessorName()
{
    return "UdpSplitter";
}