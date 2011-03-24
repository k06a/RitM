#ifndef TCPSPLITTER_H
#define TCPSPLITTER_H

#include <map>
#include "IPacket.h"
#include "CommonInclude.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
    class TcpSplitter : public AbstractProcessor
    {
        typedef std::pair<unsigned short,unsigned short> port_pair;
        typedef std::map<port_pair,IProcessorPtr> MyMap;

        MyMap Connectors;
        port_pair para;

    public:
        TcpSplitter(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;
        virtual void DestroyHierarchy();

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class TcpSplitter
}
// namespace DiplomBukov

#endif // TCPSPLITTER_H