#include "UdpSplitter.h"

using namespace DiplomBukov;

UdpSplitter::UdpSplitter(IProcessorPtr Connector)
    : TcpSplitter(Connector)
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