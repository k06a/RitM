#include "HttpSwapProcessor.h"
#include "IPacket.h"
#include <algorithm>

using namespace DiplomBukov;

HttpSwapProcessor::HttpSwapProcessor(IProcessorPtr processor)
    : markCount(0)
{
    setNextProcessor(processor);
}

IProcessorPtr HttpSwapProcessor::CreateCopy() const
{
    return IProcessorPtr(new HttpSwapProcessor(nextProcessor->CreateCopy()));
}

ProcessingStatus HttpSwapProcessor::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    // Empty packet
    if (offset == packet->size())
        return ProcessingStatus::Accepted;

    int pos = blob.size();
    if (blob.size() < packet->size() - offset)
        blob.resize(packet->size() - offset - blob.size());
    std::copy(packet->data().begin() + offset, packet->data().end(), blob.begin()+pos);

    const char * mark = "\r\n\r\n";
    MyDeque::iterator it = std::search(blob.begin(), blob.end(), mark, mark+2);
    if (it != blob.end())
        markCount++;

    if (((markCount == 1) && (blob[0] == 'G')) ||   // GET
        ((markCount == 1) && (blob[0] == 'P')) ||   // POST
        ((markCount == 2) && (blob[0] == 'H')))     // HTTP
    {
        int a = 0;
        a = a;
    }
    
    return ProcessingStatus::Accepted;

    //packet->addProcessor(Self);
    //if (nextProcessor != NULL)
    //    nextProcessor->forwardProcess("HTTP", packet, 0);
}

ProcessingStatus HttpSwapProcessor::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    return ProcessingStatus::Accepted;
}

Protocol HttpSwapProcessor::getProtocol()
{
    return Protocol("TCP_80", 80);
}

const char * HttpSwapProcessor::getProcessorName()
{
    return "HttpSwapProcessor";
}