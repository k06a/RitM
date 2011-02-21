#include "IcmpProcessor.h"
#include "icmp_header.h"

using namespace DiplomBukov;

IcmpProcessor::IcmpProcessor(IRouter * router)
    : module(NULL), router(router)
{
}

IProcessor * IcmpProcessor::CreateCopy() const
{
    return new IcmpProcessor(router->CreateCopy());
}

IPacketProcessor * IcmpProcessor::getPointer()
{
    return this;
}

void IcmpProcessor::ping(IPacketProcessor * prevProcessor)
{

}

ProcessingStatus IcmpProcessor::forwardProcess(Protocol proto, Packet & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    icmp_header * icmp = (icmp_header *)(packet.data + offset);

    //if (baseRouter != NULL)
    //    baseRouter->processPacket(Protocol::None, packet, offset + sizeof(icmp_header));

    return ProcessingStatus::Accepted;
}

ProcessingStatus IcmpProcessor::backwardProcess(Protocol proto, Packet & packet, unsigned offset)
{
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
    this->router = router;
}

IRouter * IcmpProcessor::getRouter()
{
    return router;
}

void IcmpProcessor::setModule(IProcessorModule * module)
{
    this->module = module;
}

IProcessorModule * IcmpProcessor::getModule()
{
    return module;
}