#include "BruteRouter.h"
#include "ProcessingStatus.h"
#include "IProcessor.h"

#include <algorithm>

using namespace DiplomBukov;

BruteRouter::BruteRouter()
{
}

BruteRouter::BruteRouter(const MyDeque & d)
{
    for(MyDeque::const_iterator it = d.begin(); it != d.end(); ++it)
    {
        IProcessorPtr proc = (*it)->CreateCopy();
        procList.push_back(proc);
    }
}

IProcessorPtr BruteRouter::CreateCopy() const
{
    return IProcessorPtr(new BruteRouter(procList));
}

ProcessingStatus BruteRouter::forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
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
