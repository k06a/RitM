#ifndef TCPPROCESSOR_H
#define TCPPROCESSOR_H

#include <map>
#include "IPacket.h"
#include "CommonInclude.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
    class TcpProcessor : public AbstractProcessor
    {
    public:
        TcpProcessor(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class TcpSplitter
}
// namespace DiplomBukov

#endif // TCPPROCESSOR_H