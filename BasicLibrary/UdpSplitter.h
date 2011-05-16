#ifndef UDPSPLITTER_H
#define UDPSPLITTER_H

#include "TcpSplitter.h"

namespace RitM
{
    class UdpSplitter : public TcpSplitter
    {
    public:
        UdpSplitter(ProcessorPtr Connector = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class UdpSplitter

    typedef SharedPointer<UdpSplitter>::Type UdpSplitterPtr;
}
// namespace RitM

#endif // UDPSPLITTER_H