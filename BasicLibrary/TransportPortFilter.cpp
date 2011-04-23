#include "TransportPortFilter.h"
#include "network/tcpudp_header.h"
#include "IntOption.h"

using namespace DiplomBukov;

TransportPortFilter::TransportPortFilter(ProcessorPtr Connector)
{
    setNextProcessor(Connector);
    
    IntOption * opt = new IntOption();
    opt->setMinValue(1);
    opt->setMaxValue(65535);
    opt->setIntValue(2000);
    portOption = OptionPtr(opt);
}

ProcessorPtr TransportPortFilter::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new TransportPortFilter(np));
}

ProcessingStatus TransportPortFilter::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(shared_from_this());

    tcpudp_header * header = (tcpudp_header*)&packet->data()[offset];
    IntOption * opt = (IntOption*)portOption.get();
    if (header->dst_port != opt->intValue())
        return ProcessingStatus::Rejected;

    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

const char * TransportPortFilter::getProcessorName()
{
    return "TransportPortFilter";
}

Protocol TransportPortFilter::getProtocol()
{
    return Protocol::None;
}

OptionPtr TransportPortFilter::getOptions()
{
    return portOption;
}