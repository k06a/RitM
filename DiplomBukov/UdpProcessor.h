#ifndef UDPPROCESSOR_H
#define UDPPROCESSOR_H

#include "TcpProcessor.h"

namespace DiplomBukov
{
    class UdpProcessor : public TcpProcessor
    {
    public:
        UdpProcessor(IProcessorPtr router = IProcessorPtr());

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class UdpProcessor
}
// namespace DiplomBukov

#endif // UDPPROCESSOR_H