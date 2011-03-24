#ifndef DNSMESSAGEPROCESSOR_H
#define DNSMESSAGEPROCESSOR_H

#include <string>
#include <vector>
#include <utility>
#include <sstream>

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "network\dns_header.h"

namespace DiplomBukov
{
    class DnsMessageProcessor : public AbstractProcessor
    {
        DnsMessage dnsMessage;

    public:
        DnsMessageProcessor(IProcessorPtr processor = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class DnsMessageProcessor
}
// namespace DiplomBukov

#endif // DNSMESSAGEPROCESSOR_H