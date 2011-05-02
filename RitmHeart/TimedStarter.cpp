#include "TimedStarter.h"
#include "IAdapter.h"
#include "IPacket.h"

using namespace DiplomBukov;

TimedStarter::TimedStarter()
    : m_shouldStop(false)
{
}

void TimedStarter::addAdapter(AdapterPtr adapter)
{
    adapters.push_back(adapter);
    delays.push_back(DelayInjectProcessorPtr(new DelayInjectProcessor));
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
        for (int i = 0; i < n; i++)
        {
            if (delays[i]->recvPacket.size() == 0)
                adapters[i]->tick();
            
            if (adapters[i]->type() != IAdapter::Offline)
                continue;

            if (delays[i]->recvPacket.size() > 0)
            {
                if (delays[i]->recvPacket.front()->time() < mintime)
                {
                    mintime = delays[i]->recvPacket.front()->time();
                    mintime_index = i;
                }
            }

            if (m_shouldStop)
                return;
        }

        // Если все оставшиеся процессоры типа Online
        if (mintime == 0)
        {
            for (int i = 0; i < n; i++)
                delays[i]->go();
            continue;
        }

        if (mintime_index == -1)
            break;
        delays[mintime_index]->go();
    }
}

void TimedStarter::stop()
{
    m_shouldStop = true;
}