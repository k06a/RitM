#include "TelnetSwapper.h"
#include "IPacket.h"
#include <algorithm>

using namespace DiplomBukov;

TelnetSwapper::TelnetSwapper(ProcessorPtr processor)
{
    setNextProcessor(processor);
}

ProcessorPtr TelnetSwapper::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new TelnetSwapper(np));
}

ProcessingStatus TelnetSwapper::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    //if ((proto != Protocol::None) && (proto != getProtocol()))
    //    return ProcessingStatus::Rejected;

    packet->addProcessor(shared_from_this());

    // Empty packet
    if (offset == packet->size())
        return ProcessingStatus::Accepted;

    char w[] = "hello";

    std::string text(packet->dataBegin() + offset, packet->dataEnd());
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

    if (packet->size() != offset + text.size())
    {
        packet->resize(offset + text.size());
        packet->setRealSize(offset + text.size());
    }
    std::copy(text.begin(), text.end(), &(*packet)[offset]);

    backwardProcess(proto, packet, offset);
    packet->setStatus(IPacket::Rejected);

    return ProcessingStatus::Accepted;
}

ProcessingStatus TelnetSwapper::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if (packet->processorBefore(shared_from_this()) != NULL)
        packet->processorBefore(shared_from_this())->backwardProcess(proto, packet, offset);
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

bool TelnetSwapper::isFinal()
{
    return true;
}