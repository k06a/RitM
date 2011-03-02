#ifndef UDPPROCESSOR_H
#define UDPPROCESSOR_H

#include "TcpHeaderProcessor.h"

namespace DiplomBukov
{
    class UdpProcessor : public TcpHeaderProcessor
    {
    public:
        UdpProcessor(IProcessorPtr Connector = IProcessorPtr());

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class UdpProcessor
}
// namespace DiplomBukov

#endif // UDPPROCESSOR_H