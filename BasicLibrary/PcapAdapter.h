#ifndef PCAPADAPTER_H
#define PCAPADAPTER_H

#include <stdio.h>
#include <deque>
#include <algorithm>
#include <pcap.h>

#include "AbstractProcessor.h"
#include "IAdapter.h"
#include "SwitchOption.h"
#include "IStatsProvider.h"

namespace DiplomBukov
{
    class PcapAdapter
        : public AbstractProcessor
        , public IAdapter
    {
        class StatCounter : public IStatsProvider
        {
        public:
            i64 i_count_in;
            i64 i_count_out;

            StatCounter();
            virtual int getStatistic_size() const;
            virtual i64 getStatistic_value(int i) const;
            virtual const char * getStatistic_name(int i) const;
        };

        typedef SharedPointer<StatCounter>::Type StatCounterPtr;

        StatCounterPtr statCounter;

        SwitchOptionPtr devicesSwitch;

        pcap_if_t * deviceList;
        unsigned deviceCount;
        pcap_t * device;

        unsigned id;
        int linkType;

        std::deque<u32> hashes;

    public:
        PcapAdapter(ProcessorPtr Connector = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;
        ~PcapAdapter();

        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual const char * getProcessorName();
        virtual OptionPtr getOptions();

        virtual void run(bool always);
        virtual bool tick();

        virtual Type type();

        virtual StatsProviderPtr statsProvider();
    };
    // class PcapAdapter

    typedef SharedPointer<PcapAdapter>::Type PcapAdapterPtr;
}
// namespace DiplomBukov

#endif // PCAPADAPTER_H