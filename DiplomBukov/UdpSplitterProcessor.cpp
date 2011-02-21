#include "UdpSplitterProcessor.h"

using namespace DiplomBukov;

UdpSplitterProcessor::UdpSplitterProcessor(IPacketProcessor * router)
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