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
        IOptionPtr devicesSwitch;
        std::deque<IOptionPtr> options;

        pcap_if_t * deviceList;
        unsigned deviceCount;
        pcap_if_t * device;

    public:
        PcapAdapter(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;
        ~PcapAdapter();

        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual const char * getProcessorName();
        virtual std::deque<IOptionPtr> getOptions();

        virtual void run();
    };
    // class PcapAdapter
}
// namespace DiplomBukov

#endif // PCAPADAPTER_H