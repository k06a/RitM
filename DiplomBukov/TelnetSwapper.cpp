#include "TelnetSwapper.h"
#include "IPacket.h"
#include <algorithm>

using namespace DiplomBukov;

TelnetSwapper::TelnetSwapper(IProcessorPtr processor)
{
    setNextProcessor(processor);
}

IProcessorPtr TelnetSwapper::CreateCopy() const
{
    IProcessorPtr ptr(new TelnetSwapper(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus TelnetSwapper::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    //if ((proto != Protocol::None) && (proto != getProtocol()))
    //    return ProcessingStatus::Rejected;

    packet->addProcessor(Self);

    // Empty packet
    if (offset == packet->size())
        return ProcessingStatus::Accepted;

    char w[] = "fuck";

    std::string line((char*)&packet->data()[offset]);
    std::string::iterator it = std::search(line.begin(), line.end(), w, w+4);
    if (it != line.end())
    {
        *(++it) = '*';
        *(++it) = '*';
    }
    std::copy(line.begin(), line.end(), &packet->data()[offset]);

    backwardProcess(proto, packet, offset);
    packet->setStatus(IPacket::Rejected);

    return ProcessingStatus::Accepted;
}

ProcessingStatus TelnetSwapper::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if (packet->prevProcessor(Self) != NULL)
        packet->prevProcessor(Self)->backwardProcess(proto, packet, offset);
    return ProcessingStatus::Accepted;
}

Protocol TelnetSwapper::getProtocol()
{
    return Protocol::None;
}

const char * TelnetSwapper::getProcessorName()
{
    return "TelnetSwapper";
}