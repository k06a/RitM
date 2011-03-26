#include "TransportPortFilter.h"
#include "network/tcpudp_header.h"
#include "IntOption.h"

using namespace DiplomBukov;

TransportPortFilter::TransportPortFilter(IProcessorPtr Connector)
{
    setNextProcessor(Connector);
    
    IntOption * opt = new IntOption();
    opt->setMinValue(1);
    opt->setMaxValue(65535);
    opt->setIntValue(2000);
    portOption = IOptionPtr(opt);
}

IProcessorPtr TransportPortFilter::CreateCopy() const
{
    IProcessorPtr ptr(new TransportPortFilter(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus TransportPortFilter::forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(Self);

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

IOptionPtr TransportPortFilter::getOptions()
{
    return portOption;
}