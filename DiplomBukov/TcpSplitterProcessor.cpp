#include "TcpSplitterProcessor.h"
#include "tcp_header.h"

using namespace DiplomBukov;

TcpSplitterProcessor::TcpSplitterProcessor(IRouter * router_)
: router_(router_)
{
}

IProcessor * TcpSplitterProcessor::CreateCopy() const
{
    return new TcpSplitterProcessor(router_->CreateCopy());
}

ProcessingStatus TcpSplitterProcessor::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != Protocol::TCP))
        return ProcessingStatus::Rejected;

    tcp_header * tcp = (tcp_header *)(packet.data + offset);

    if (router_ != 0)
        router_->transmitPacket(Protocol::None, packet, offset + tcp->header_size());

    return ProcessingStatus::Accepted;
}

Protocol TcpSplitterProcessor::getProtocol()
{
    return Protocol::TCP;
}

void TcpSplitterProcessor::setRouter(IRouter * router_)
{
    this->router_ = router_;
}

IRouter * TcpSplitterProcessor::router()
{
    return router_;
}