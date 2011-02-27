#include "BruteConnector.h"
#include "ProcessingStatus.h"
#include "IProcessor.h"

#include <algorithm>

using namespace DiplomBukov;

BruteConnector::BruteConnector()
{
}

BruteConnector::BruteConnector(const MyDeque & d)
{
    for(MyDeque::const_iterator it = d.begin(); it != d.end(); ++it)
    {
        IProcessorPtr proc = (*it)->CreateCopy();
        procList.push_back(proc);
    }
}

IProcessorPtr BruteConnector::CreateCopy() const
{
    return IProcessorPtr(new BruteConnector(procList));
}

ProcessingStatus BruteConnector::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
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
