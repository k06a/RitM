#include "UdpProcessor.h"

using namespace DiplomBukov;

UdpProcessor::UdpProcessor(IProcessorPtr Connector)
    : TcpHeaderProcessor(Connector)
{
}

Protocol UdpProcessor::getProtocol()
{
    return Protocol::UDP;
}

const char * UdpProcessor::getProcessorName()
{
    return "UdpProcessor";
}