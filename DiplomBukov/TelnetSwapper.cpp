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

    char w[] = "hello";

    std::string text(packet->data().begin() + offset, packet->data().end());
    std::string::iterator it = std::search(text.begin(), text.end(), w, w+4);
    bool cut = false;
    if (it != text.end())
    {
        (*it++) = 'b';
        (*it++) = 'y';
        (*it++) = 'e';
        text.erase(it, it+2);
        cut = true;
    }

    if (packet->data().size() != offset + text.size())
    {
        packet->data().resize(offset + text.size());
        packet->setRealSize(offset + text.size());
    }
    std::copy(text.begin(), text.end(), &packet->data()[offset]);

    backwardProcess(proto, packet, offset);
    packet->setStatus(IPacket::Rejected);

    return ProcessingStatus::Accepted;
}

ProcessingStatus TelnetSwapper::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if (packet->processorBefore(Self) != NULL)
        packet->processorBefore(Self)->backwardProcess(proto, packet, offset);
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