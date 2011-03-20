#include "TcpOptionsRemover.h"

using namespace DiplomBukov;

TcpOptionsRemover::TcpOptionsRemover(IProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

IProcessorPtr TcpOptionsRemover::CreateCopy() const
{
    IProcessorPtr ptr(new TcpOptionsRemover(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus TcpOptionsRemover::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    tcp_header * tcp = (tcp_header *)(&packet->data()[offset]);
    if (tcp->header_size() > sizeof(tcp_header))
    {
        packet->data().erase(
            packet->data().begin() + offset + sizeof(tcp_header),
            packet->data().begin() + offset + tcp->header_size());
        packet->setRealSize(packet->size());
        tcp->set_header_size(sizeof(tcp_header));
    }
    
    packet->addProcessor(Self);
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpOptionsRemover::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if (packet->prevProcessor(Self) != NULL)
        packet->prevProcessor(Self)->backwardProcess(Protocol::TCP, packet, offset);

    return ProcessingStatus::Accepted;
}

const char * TcpOptionsRemover::getProcessorName()
{
    return "TcpOptionsRemover";
}

Protocol TcpOptionsRemover::getProtocol()
{
    return Protocol::TCP;
}
