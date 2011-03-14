#include "TcpSplitter.h"
#include "tcp_header.h"

using namespace DiplomBukov;

TcpSplitter::TcpSplitter(IProcessorPtr Connector)
{
    setNextProcessor(Connector);
}

IProcessorPtr TcpSplitter::CreateCopy() const
{
    IProcessorPtr ptr(new TcpSplitter(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus TcpSplitter::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    tcp_header * tcp = (tcp_header *)(&packet->data()[0] + offset);
    unsigned short adr1 = tcp->src_port;
    unsigned short adr2 = tcp->dst_port;
    
    // Create new Connector if needed
    port_pair para1(adr1,adr2);
    port_pair para2(adr2,adr1);
    MyMap::iterator it1 = Connectors.find(para1);
    MyMap::iterator it2 = Connectors.find(para2);
    para = (it1 == Connectors.end()) ? para2 : para1;
    if ((it1 == Connectors.end()) && (it2 == Connectors.end()))
    {
        if (nextProcessor != NULL)
            Connectors[para] = nextProcessor->CreateCopy();
    }

    // Determine direction
    if ((packet->direction() == IPacket::Unknown) && (adr1 != adr2))
    {
        bool cts = (adr1 == para.first);
        packet->setDirection(cts ? IPacket::ClientToServer : IPacket::ServerToClient);
    }

    packet->addProcessor(Self);
    if (nextProcessor != NULL)
        Connectors[para]->forwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus TcpSplitter::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    tcp_header * tcp = (tcp_header *)(&packet->data()[0] + offset);

    tcp->src_port = para.first;
    tcp->dst_port = para.second;

    if (packet->direction() == IPacket::ServerToClient)
        std::swap(tcp->src_port, tcp->dst_port);

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
