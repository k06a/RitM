#include "TimedStarter.h"
#include "IAdapter.h"
#include "IPacket.h"

using namespace RitM;

TimedStarter::TimedStarter()
    : m_shouldStop(false)
{
}

void TimedStarter::addAdapter(AdapterPtr adapter)
{
    adapters.push_back(adapter);
    delays.push_back(DelayInjectProcessorPtr(new DelayInjectProcessor));
}

void TimedStarter::clearAdapters()
{
    adapters.clear();
    delays.clear();
}

void TimedStarter::start()
{
    int n = adapters.size();

    for (int i = 0; i < n; i++)
    {
        delays[i]->pickUpTo(adapters[i]);
        adapters[i]->run(false);
    }
    
    while (true)
    {
        u64 mintime = ULLONG_MAX;
        int mintime_index = -1;
        bool allFalse = true;
        for (int i = 0; i < n; i++)
        {
            if (m_shouldStop)
                return;

            if (delays[i]->recvPacket.size() == 0)
                allFalse &= !adapters[i]->tick();
            else
                allFalse = false;
        
            if (delays[i]->recvPacket.size() == 0)
                continue;

            if (delays[i]->recvPacket.front()->time() < mintime)
            {
                mintime = delays[i]->recvPacket.front()->time();
                mintime_index = i;
            }
        }

        if (allFalse)
            break;

        if (mintime_index == -1)
            continue;

        delays[mintime_index]->go();
    }
}

void TimedStarter::stop()
{
    m_shouldStop = true;
}