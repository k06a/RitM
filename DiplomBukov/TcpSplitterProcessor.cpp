#include "TcpSplitterProcessor.h"
#include "tcp_header.h"

using namespace DiplomBukov;

TcpSplitterProcessor::TcpSplitterProcessor(IProcessorPtr router)
{
    setNextProcessor(router);
}

IProcessorPtr TcpSplitterProcessor::CreateCopy() const
{
    return IProcessorPtr(new TcpSplitterProcessor(nextProcessor->CreateCopy()));
}

ProcessingStatus TcpSplitterProcessor::forwardProcess(Protocol proto, Packet & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    tcp_header * tcp = (tcp_header *)(packet.data + offset);

    unsigned short adr1 = tcp->src_port;
    unsigned short adr2 = tcp->dst_port;
    if (adr2 < adr1) std::swap(adr1, adr2);

    // Create new router if needed
    port_pair para(adr1,adr2);
    MyMap::iterator it = routers.find(para);
    if (it == routers.end())
    {
        if (nextProcessor != NULL)
            routers[para] = nextProcessor->CreateCopy();
    }

    if (nextProcessor != NULL)
        routers[para]->forwardProcess(Protocol::None, packet, offset + tcp->header_size());

    return ProcessingStatus::Accepted;
}

const char * TcpSplitterProcessor::getProcessorName()
{
    return "TcpSplitterProcessor";
}

Protocol TcpSplitterProcessor::getProtocol()
{
    return Protocol::TCP;
}
