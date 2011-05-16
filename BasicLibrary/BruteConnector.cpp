#include "BruteConnector.h"
#include "ProcessingStatus.h"
#include "IProcessor.h"

#include <algorithm>

using namespace RitM;

BruteConnector::BruteConnector()
{
}

BruteConnector::BruteConnector(const MyDeque & d)
{
    for(MyDeque::const_iterator it = d.begin(); it != d.end(); ++it)
    {
        ProcessorPtr proc = (*it)->CreateCopy();
        procList.push_back(proc);
    }
}

ProcessorPtr BruteConnector::CreateCopy() const
{
    return ProcessorPtr(new BruteConnector(procList));
}

ProcessingStatus BruteConnector::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    ProcessingStatus ans = ProcessingStatus::Rejected;
    for(MyDeque::iterator it = procList.begin(); it != procList.end(); it++)
    {
        ProcessingStatus ret = (*it)->forwardProcess(proto, packet, offset);
        if (ret == ProcessingStatus::Accepted)
            ans = ProcessingStatus::Accepted;
    }
    return ans;
}
