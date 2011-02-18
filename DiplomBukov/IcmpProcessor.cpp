#include "IcmpProcessor.h"
#include "icmp_header.h"

using namespace DiplomBukov;

IcmpProcessor::IcmpProcessor(IRouter * router)
    : module(NULL), baseRouter(router)
{
}

IProcessor * IcmpProcessor::CreateCopy() const
{
    return new IcmpProcessor(baseRouter->CreateCopy());
}

ProcessingStatus IcmpProcessor::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    icmp_header * icmp = (icmp_header *)(packet.data + offset);

    //if (baseRouter != NULL)
    //    baseRouter->transmitPacket(Protocol::None, packet, offset + sizeof(icmp_header));

    return ProcessingStatus::Accepted;
}

const char * IcmpProcessor::getProcessorName()
{
    return "IcmpProcessor";
}

Protocol IcmpProcessor::getProtocol()
{
    return Protocol::TCP;
}

void IcmpProcessor::setRouter(IRouter * router)
{
    baseRouter = router;
}

IRouter * IcmpProcessor::getRouter()
{
    return baseRouter;
}

void IcmpProcessor::setModule(IProcessorModule * module)
{
    this->module = module;
}

IProcessorModule * IcmpProcessor::getModule()
{
    return module;
}