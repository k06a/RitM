#ifndef TCPOPTIONSREMOVER_H
#define TCPOPTIONSREMOVER_H

#include <map>
#include <vector>

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IPacket.h"
#include "tcp_header.h"

namespace DiplomBukov
{
    class TcpOptionsRemover : public AbstractProcessor
    {
    public:
        TcpOptionsRemover(IProcessorPtr Connector = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class TcpOptionsRemover
}
// namespace DiplomBukov

#endif // TCPOPTIONSREMOVER_H