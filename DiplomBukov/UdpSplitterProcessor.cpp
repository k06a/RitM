#include "UdpSplitterProcessor.h"

using namespace DiplomBukov;

UdpSplitterProcessor::UdpSplitterProcessor(IProcessorPtr router)
    : TcpSplitterProcessor(router)
{
}

Protocol UdpSplitterProcessor::getProtocol()
{
    return Protocol::UDP;
}

const char * UdpSplitterProcessor::getProcessorName()
{
    return "UdpSplitterProcessor";
}