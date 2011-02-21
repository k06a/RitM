#include "IcmpProcessor.h"
#include "icmp_header.h"

using namespace DiplomBukov;

IcmpProcessor::IcmpProcessor(IPacketProcessor * router)
{
    setNextProcessor(router);
}

IProcessor * IcmpProcessor::CreateCopy() const
{
    return new IcmpProcessor(nextProcessor->CreateCopy());
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

const char * IcmpProcessor::getProcessorName()
{
    return "IcmpProcessor";
}

Protocol IcmpProcessor::getProtocol()
{
    return Protocol::TCP;
}