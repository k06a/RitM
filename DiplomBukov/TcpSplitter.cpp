#include "TcpSplitter.h"
#include "tcp_header.h"

using namespace DiplomBukov;

TcpSplitter::TcpSplitter(IProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

IProcessorPtr TcpSplitter::CreateCopy() const
{
    return IProcessorPtr(new TcpSplitter(nextProcessor->CreateCopy()));
}

ProcessingStatus TcpSplitter::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(Self);

    tcp_header * tcp = (tcp_header *)(packet->data() + offset);

    unsigned short adr1 = tcp->src_port;
    unsigned short adr2 = tcp->dst_port;
    if (adr2 < adr1) std::swap(adr1, adr2);

    // Create new Connector if needed
    port_pair para(adr1,adr2);
    MyMap::iterator it = Connectors.find(para);
    if (it == Connectors.end())
    {
        if (nextProcessor != NULL)
            Connectors[para] = nextProcessor->CreateCopy();
    }

    if (nextProcessor != NULL)
        Connectors[para]->forwardProcess(Protocol::None, packet, offset + tcp->header_size());

    return ProcessingStatus::Accepted;
}

const char * TcpSplitter::getProcessorName()
{
    return "TcpSplitter";
}

Protocol TcpSplitter::getProtocol()
{
    return Protocol::TCP;
}
