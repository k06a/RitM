#ifndef PCAPADAPTER_H
#define PCAPADAPTER_H

#include <stdio.h>
#include <deque>
#include <algorithm>
#include <pcap.h>

#include "AbstractProcessor.h"
#include "IAdapter.h"
#include "SwitchOption.h"
#include "BasicStatCounter.h"

namespace RitM
{
    class PcapAdapter
        : public AbstractProcessor
        , public IAdapter
    {
        BasicStatCounterPtr statCounter;

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
// namespace RitM

#endif // PCAPADAPTER_H