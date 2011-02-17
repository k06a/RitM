#include "Ipv4DefragProcessor.h"
#include "ipv4_header.h"

using namespace DiplomBukov;

Ipv4DefragProcessor::Ipv4DefragProcessor(IRouter * router)
	: baseRouter(router), fullPacket(NULL)
{
}

IProcessor * Ipv4DefragProcessor::CreateCopy() const
{
    return new Ipv4DefragProcessor(baseRouter->CreateCopy());
}

ProcessingStatus Ipv4DefragProcessor::processPacket(Protocol proto, Packet & packet, unsigned offset)
{
    if (baseRouter == NULL)
        return ProcessingStatus::Rejected;

    ipv4_header * ipv4 = (ipv4_header *)(packet.data + offset);

    if (ipv4->flag_mf || (ipv4->fragmentOffset() != 0))
    {
        if (fullPacket == NULL)
        {
            fullPacket = new Packet(Packet::Ipv4PacketSize, true);
            ipDataOffset = offset + ipv4->size();
            fullPacket->append(0, packet.data, ipDataOffset, ipv4->flag_mf);
        }
        
        fullPacket->append(ipDataOffset + ipv4->fragmentOffset(),
                           ipv4->data, ipv4->totalLength, ipv4->flag_mf);
        packet.status = Packet::Rejected;

        if (fullPacket->finished())
        {
            ipv4_header * newIp = (ipv4_header *)(fullPacket->data + offset);
            newIp->flag_mf = 0;
            newIp->setFragmentOffset(0);
            newIp->totalLength = ipv4->fragmentOffset() + ipv4->totalLength;
            fullPacket->real_size = offset + newIp->size() + newIp->totalLength;
            newIp->recountSum();

            packet.reinitFrom(*fullPacket);
            delete fullPacket;
            fullPacket = NULL;

            if (baseRouter != 0)
                baseRouter->transmitPacket(ipv4->proto, packet, offset + ipv4->size());
        }
    }

	return ProcessingStatus::Accepted;
}

Protocol Ipv4DefragProcessor::getProtocol()
{
    return Protocol::IPv4;
}

const char * Ipv4DefragProcessor::getProcessorName()
{
    return "Ipv4DefragProcessor";
}

void Ipv4DefragProcessor::setRouter(IRouter * router)
{
	baseRouter = router;
}

IRouter * Ipv4DefragProcessor::router()
{
	return baseRouter;
}