#include "Ipv4Defragger.h"
#include "ipv4_header.h"

using namespace DiplomBukov;

Ipv4Defragger::Ipv4Defragger(IProcessorPtr router)
	: fullPacket(NULL)
{
    setNextProcessor(router);
}

IProcessorPtr Ipv4Defragger::CreateCopy() const
{
    return IProcessorPtr(new Ipv4Defragger(nextProcessor->CreateCopy()));
}

ProcessingStatus Ipv4Defragger::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;
    
    packet->addProcessor(Self);

    ipv4_header * ipv4 = (ipv4_header *)(packet->data() + offset);

    if (ipv4->flag_mf || (ipv4->fragmentOffset() != 0))
    {
        if (fullPacket == NULL)
        {
            fullPacket = new DefragPacket();
            ipDataOffset = offset + ipv4->size();
            fullPacket->append(0, packet->data(), ipDataOffset, ipv4->flag_mf);
        }
        
        fullPacket->append(ipDataOffset + ipv4->fragmentOffset(),
                           ipv4->data, ipv4->totalLength - ipv4->size(),
                           ipv4->flag_mf);
        packet->setStatus(IPacket::Rejected);

        if (fullPacket->finished())
        {
            ipv4_header * newIp = (ipv4_header *)(fullPacket->pack->data() + offset);
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
                nextProcessor->forwardProcess(ipv4->proto, packet, offset + ipv4->size());
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
