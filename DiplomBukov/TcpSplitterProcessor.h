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
        typedef std::map<port_pair,IPacketProcessor*> MyMap;

        MyMap routers;

    public:
        TcpSplitterProcessor(IPacketProcessor * router = NULL);
        virtual IProcessor * CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class TcpSplitterProcessor
}
// namespace DiplomBukov

#endif // TCPSPLITTERPROCESSOR_H