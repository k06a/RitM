#ifndef UDPSPLITTER_H
#define UDPSPLITTER_H

#include "TcpSplitter.h"

namespace DiplomBukov
{
    class UdpSplitter : public TcpSplitter
    {
    public:
        UdpSplitter(ProcessorPtr Connector = ProcessorPtr());

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class UdpSplitter
}
// namespace DiplomBukov

#endif // UDPSPLITTER_H