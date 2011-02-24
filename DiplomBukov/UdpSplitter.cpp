#include "UdpSplitter.h"

using namespace DiplomBukov;

UdpSplitter::UdpSplitter(IProcessorPtr router)
    : TcpSplitter(router)
{
}

Protocol UdpSplitter::getProtocol()
{
    return Protocol::UDP;
}

const char * UdpSplitter::getProcessorName()
{
    return "UdpSplitter";
}