#include "Ipv4Defragger.h"
#include "network/ipv4_header.h"

using namespace DiplomBukov;

Ipv4Defragger::Ipv4Defragger(ProcessorPtr Connector)
	: fullPacket(NULL)
{
    setNextProcessor(Connector);
}

ProcessorPtr Ipv4Defragger::CreateCopy() const
{
    ProcessorPtr ptr(new Ipv4Defragger(nextProcessor->CreateCopy()));
    return ptr;
}

ProcessingStatus Ipv4Defragger::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;
    
    packet->addProcessor(this->shared_from_this());

    ipv4_header_data * ipv4 = (ipv4_header_data *)(&packet->data()[offset]);

    if (ipv4->flag_mf || (ipv4->fragmentOffset() != 0))
    {
        if (fullPacket == NULL)
        {
            fullPacket = new DefragPacket();
            ipDataOffset = offset + ipv4->size();
            fullPacket->append(0, &packet->data()[0], ipDataOffset, ipv4->flag_mf);
        }
        
        fullPacket->append(ipDataOffset + ipv4->fragmentOffset(),
                           ipv4->data, ipv4->totalLength - ipv4->size(),
                           ipv4->flag_mf);
        packet->setStatus(IPacket::Rejected);

        if (fullPacket->finished())
        {
            ipv4_header * newIp = (ipv4_header *)(&fullPacket->pack->data()[0] + offset);
            newIp->flag_mf = 0;
            newIp->setFragmentOffset(0);
            newIp->totalLength = ipv4->fragmentOffset() + ipv4->totalLength;
            fullPacket->pack->setRealSize(offset + newIp->totalLength);
            newIp->recountSum();

            //packet.reinitFrom(*fullPacket);
            packet = fullPacket->pack;
            delete fullPacket;
            fullPacket = NULL;

            if (nextProcessor != 0)
            {
                Protocol::TransportLayer prot = (Protocol::TransportLayer)ipv4->proto;
                nextProcessor->forwardProcess(prot, packet, offset + ipv4->size());
            }
        }
    }

	return ProcessingStatus::Accepted;
}

Protocol Ipv4Defragger::getProtocol()
{
    return Protocol::IPv4;
}

const char * Ipv4Defragger::getProcessorName()
{
    return "Ipv4Defragger";
}
