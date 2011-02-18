#ifndef UDPSPLITTERPROCESSOR_H
#define UDPSPLITTERPROCESSOR_H

#include "TcpSplitterProcessor.h"

namespace DiplomBukov
{
    class UdpSplitterProcessor : public TcpSplitterProcessor
    {
    public:
        UdpSplitterProcessor(IRouter * router = NULL);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class UdpSplitterProcessor
}
// namespace DiplomBukov

#endif // UDPSPLITTERPROCESSOR_H