#include "TcpSplitter.h"
#include "network/tcp_header.h"

using namespace DiplomBukov;

TcpSplitter::TcpSplitter(ProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

ProcessorPtr TcpSplitter::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new TcpSplitter(np));
}

void TcpSplitter::DestroyHierarchy()
{
    for (MyMap::iterator it = Connectors.begin();
        it != Connectors.end(); ++it)
    {
        it->second->DestroyHierarchy();
    }
    Connectors.clear();
    AbstractProcessor::DestroyHierarchy();
}

ProcessingStatus TcpSplitter::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    tcp_header * tcp = (tcp_header *)(&packet->data()[offset]);
    unsigned short adr1 = tcp->src_port;
    unsigned short adr2 = tcp->dst_port;
    
    // Create new Connector if needed
    port_pair para1(adr1,adr2);
    port_pair para2(adr2,adr1);
    MyMap::iterator it1 = Connectors.find(para1);
    MyMap::iterator it2 = Connectors.find(para2);
    if ((it1 == Connectors.end()) && (it2 == Connectors.end()))
    {
        para = para1;
        if (nextProcessor != NULL)
            Connectors[para] = nextProcessor->CreateCopy();
    }

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

ProcessingStatus TcpSplitter::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    tcp_header * tcp = (tcp_header *)(&packet->data()[offset]);

    tcp->src_port = para.first;
    tcp->dst_port = para.second;

    //if (packet->direction() == IPacket::ServerToClient)
    //    std::swap(tcp->src_port, tcp->dst_port);

    if (packet->processorBefore(this->shared_from_this()) != NULL)
        packet->processorBefore(this->shared_from_this())->backwardProcess(Protocol::TCP, packet, offset);

    return ProcessingStatus::Accepted;
}

const char * TcpSplitter::getProcessorName()
{
    return "TcpSplitter";
}

Protocol TcpSplitter::getProtocol()
{
    return Protocol::TCP;
}
