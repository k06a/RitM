#include "Ipv4Splitter.h"
#include "network/ipv4_header.h"

using namespace DiplomBukov;

Ipv4Splitter::Ipv4Splitter(ProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

ProcessorPtr Ipv4Splitter::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new Ipv4Splitter(np));
}

void Ipv4Splitter::DestroyHierarchy()
{
    for (MyMap::iterator it = Connectors.begin();
        it != Connectors.end(); ++it)
    {
        it->second->DestroyHierarchy();
    }
    Connectors.clear();
    AbstractProcessor::DestroyHierarchy();
}

ProcessingStatus Ipv4Splitter::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    ipv4_header * ipv4 = (ipv4_header *)(&packet->data()[offset]);
    ipv4_addr adr1 = ipv4->src_data;
    ipv4_addr adr2 = ipv4->dst_data;

    // Create new Connector if needed
    ipv4_pair para1(adr1,adr2);
    ipv4_pair para2(adr2,adr1);
    MyMap::iterator it1 = Connectors.find(para1);
    MyMap::iterator it2 = Connectors.find(para2);
    if ((it1 == Connectors.end()) && (it2 == Connectors.end()))
    {
        para = para1;
        if (nextProcessor != NULL)
        {
            Connectors[para] = nextProcessor->CreateCopy();
            it1 = Connectors.find(para1);
        }
    } else
        para = (it1 != Connectors.end()) ? para1 : para2;
    
    // Determine direction
    if ((packet->direction() == IPacket::Unknown) && (adr1 != adr2))
    {
        bool cts = (adr1 == para.first);
        packet->setDirection(cts ? IPacket::ClientToServer : IPacket::ServerToClient);
    }

    packet->addProcessor(this->shared_from_this());
    if (nextProcessor != NULL)
        Connectors[para]->forwardProcess(proto, packet, offset);

	return ProcessingStatus::Accepted;
}

ProcessingStatus Ipv4Splitter::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    ipv4_header * ipv4 = (ipv4_header *)(&packet->data()[offset]);

    ipv4->src_data = para.first;
    ipv4->dst_data = para.second;

    //if (packet->direction() == IPacket::ServerToClient)
    //    std::swap(ipv4->src_data, ipv4->dst_data);

    if (packet->processorBefore(this->shared_from_this()) != NULL)
        packet->processorBefore(this->shared_from_this())->backwardProcess(Protocol::IPv4, packet, offset);

    return ProcessingStatus::Accepted;
}

Protocol Ipv4Splitter::getProtocol()
{
    return Protocol::IPv4;
}

const char * Ipv4Splitter::getProcessorName()
{
    return "Ipv4Splitter";
}
