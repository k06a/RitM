#include "TcpOptionsRemover.h"

using namespace RitM;

TcpOptionsRemover::TcpOptionsRemover(ProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

ProcessorPtr TcpOptionsRemover::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new TcpOptionsRemover(np));
}

ProcessingStatus TcpOptionsRemover::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    tcp_header * tcp = (tcp_header *)(&(*packet)[offset]);
    if (tcp->header_size() > sizeof(tcp_header))
    {
        packet->erase(
            offset + sizeof(tcp_header),
            offset + tcp->header_size());
        packet->setRealSize(packet->size());
        tcp->set_header_size(sizeof(tcp_header));
    }
    
    packet->addProcessor(shared_from_this());
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpOptionsRemover::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if (packet->processorBefore(shared_from_this()) != NULL)
        packet->processorBefore(shared_from_this())->backwardProcess(Protocol::TCP, packet, offset);

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
