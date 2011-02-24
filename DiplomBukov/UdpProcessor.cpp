#include "UdpProcessor.h"

using namespace DiplomBukov;

UdpProcessor::UdpProcessor(IProcessorPtr router)
    : TcpProcessor(router)
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