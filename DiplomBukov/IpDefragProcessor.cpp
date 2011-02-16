#include "IpDefragProcessor.h"
#include "ipv4_header.h"

using namespace DiplomBukov;

IpDefragProcessor::IpDefragProcessor(IRouter * router)
	: baseRouter(router)
{
}

IProcessor * IpDefragProcessor::CreateCopy() const
{
    return new IpDefragProcessor(baseRouter->CreateCopy());
}

ProcessingStatus IpDefragProcessor::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
    if (baseRouter == NULL)
        return ProcessingStatus::Rejected;

    ipv4_header * ipv4 = (ipv4_header *)(packet.data + offset);
    
	if (baseRouter != 0)
		baseRouter->transmitPacket(ipv4->proto, packet, offset + ipv4->size());

	return ProcessingStatus::Accepted;
}

Protocol IpDefragProcessor::getProtocol()
{
    return Protocol::IP;
}

void IpDefragProcessor::setRouter(IRouter * router)
{
	baseRouter = router;
}

IRouter * IpDefragProcessor::router()
{
	return baseRouter;
}