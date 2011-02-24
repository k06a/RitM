#ifndef TCPSPLITTERPROCESSOR_H
#define TCPSPLITTERPROCESSOR_H

#include <map>
#include "CommonInclude.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
    class TcpSplitterProcessor : public AbstractProcessor
    {
        typedef std::pair<unsigned short,unsigned short> port_pair;
        typedef std::map<port_pair,IProcessorPtr> MyMap;

        MyMap routers;

    public:
        TcpSplitterProcessor(IProcessorPtr router = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class TcpSplitterProcessor
}
// namespace DiplomBukov

#endif // TCPSPLITTERPROCESSOR_H