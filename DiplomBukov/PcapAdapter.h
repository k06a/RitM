#ifndef PCAPADAPTER_H
#define PCAPADAPTER_H

#include <stdio.h>
#include <deque>
#include <algorithm>
#include <pcap.h>

#include "AbstractProcessor.h"
#include "IAdapter.h"
#include "SwitchOption.h"

namespace DiplomBukov
{
    class PcapAdapter
        : public AbstractProcessor
        , public IAdapter
    {
        SwitchOptionPtr devicesSwitch;

        pcap_if_t * deviceList;
        unsigned deviceCount;
        pcap_t * device;

        unsigned id;
        int linkType;

        std::deque<u32> hashes;

    public:
        PcapAdapter(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;
        ~PcapAdapter();

        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);

        virtual const char * getProcessorName();
        virtual IOptionPtr getOptions();

        virtual void run(bool always);
        virtual bool tick();
    };
    // class PcapAdapter
}
// namespace DiplomBukov

#endif // PCAPADAPTER_H