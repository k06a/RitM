#ifndef DNSMESSAGEPROCESSOR_H
#define DNSMESSAGEPROCESSOR_H

#include <string>
#include <vector>
#include <utility>
#include <sstream>

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "network\dns_header.h"
#include "SwitchOption.h"
#include "TextLineOption.h"
#include "ListOption.h"
#include "GroupOption.h"
#include "CheckOption.h"

namespace DiplomBukov
{
    class DnsMessageProcessor : public AbstractProcessor
    {
        DnsMessage dnsMessage;
        CheckOption * check;
        TextLineOption * source;
        SwitchOption * destType;
        TextLineOption * destination;
        GroupOptionPtr options;

    public:
        DnsMessageProcessor(ProcessorPtr processor = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();

        virtual OptionPtr getOptions();
    };
    // class DnsMessageProcessor
}
// namespace DiplomBukov

#endif // DNSMESSAGEPROCESSOR_H