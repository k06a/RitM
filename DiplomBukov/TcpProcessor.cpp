#include "TcpProcessor.h"
#include "tcp_header.h"

using namespace DiplomBukov;

TcpProcessor::TcpProcessor(IRouter * router_)
: router_(router_)
{
}

IProcessor * TcpProcessor::CreateCopy() const
{
    return new TcpProcessor(router_->CreateCopy());
}

ProcessingStatus TcpProcessor::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != Protocol::TCP))
        return ProcessingStatus::Rejected;

    tcp_header * tcp = (tcp_header *)(packet.data + offset);

    if (router_ != 0)
        router_->transmitPacket(Protocol::None, packet, offset + tcp->header_size());

    return ProcessingStatus::Accepted;
}

Protocol TcpProcessor::getProtocol()
{
    return Protocol::TCP;
}

void TcpProcessor::setRouter(IRouter * router_)
{
    this->router_ = router_;
}

IRouter * TcpProcessor::router()
{
    return router_;
}