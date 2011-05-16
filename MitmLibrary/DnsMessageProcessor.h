#ifndef DNSMESSAGEPROCESSOR_H
#define DNSMESSAGEPROCESSOR_H

#include <string>
#include <vector>
#include <utility>
#include <sstream>

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "network/dns_header.h"
#include "SwitchOption.h"
#include "ComboOption.h"
#include "TextLineOption.h"
#include "ListOption.h"
#include "GroupOption.h"
#include "CheckOption.h"
#include "PushButtonOption.h"

namespace DiplomBukov
{
    class DnsMessageProcessor : public AbstractProcessor
    {

        class AppendLitener : public IPushButtonOptionListener
        {
            GroupOptionPtr groupOption;

        public:
            AppendLitener(GroupOptionPtr groupOption);
            virtual void clicked();
        };
        // AppendLitener
        
        //

        class RemoveLitener : public IPushButtonOptionListener
        {
            GroupOptionPtr groupOption;

        public:
            RemoveLitener(GroupOptionPtr groupOption);
            virtual void clicked();
        };
        // RemoveLitener

        //

        DnsMessage dnsMessage;
        CheckOptionPtr alwaysResave;
        CheckOptionPtr check;
        TextLineOptionPtr source;
        ComboOptionPtr destType;
        TextLineOptionPtr destination;
        GroupOptionPtr options_list;
        GroupOptionPtr options;

        PushButtonOptionListenerPtr appendListener;
        PushButtonOptionListenerPtr removeListener;

    public:
        DnsMessageProcessor(ProcessorPtr processor = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();

        virtual OptionPtr getOptions();

        virtual bool isFinal();
    };
    // class DnsMessageProcessor

    typedef SharedPointer<DnsMessageProcessor>::Type DnsMessageProcessorPtr;
}
// namespace DiplomBukov

#endif // DNSMESSAGEPROCESSOR_H