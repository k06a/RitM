#ifndef TCPOPTIONSREMOVER_H
#define TCPOPTIONSREMOVER_H

#include <map>
#include <vector>

#include "CommonInclude.h"
#include "AbstractProcessor.h"
#include "IPacket.h"
#include "network/tcp_header.h"

namespace RitM
{
    class TcpOptionsRemover : public AbstractProcessor
    {
    public:
        TcpOptionsRemover(ProcessorPtr Connector = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class TcpOptionsRemover

    typedef SharedPointer<TcpOptionsRemover>::Type TcpOptionsRemoverPtr;
}
// namespace RitM

#endif // TCPOPTIONSREMOVER_H