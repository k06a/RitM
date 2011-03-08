#ifndef PCAPADAPTER_H
#define PCAPADAPTER_H

#include <stdio.h>

#include "AbstractProcessor.h"
#include "IAdapter.h"
#include <pcap.h>

namespace DiplomBukov
{
    class PcapAdapter
        : public AbstractProcessor
        , public IAdapter
    {
        pcap_if_t * deviceList;
        pcap_if_t * device;

    public:
        PcapAdapter(std::string adapterName, IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;
        ~PcapAdapter();

        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual const char * getProcessorName();

        virtual void run();
    };
    // class PcapAdapter
}
// namespace DiplomBukov

#endif // PCAPADAPTER_H