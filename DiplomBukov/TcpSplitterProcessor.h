#ifndef TCPSPLITTERPROCESSOR_H
#define TCPSPLITTERPROCESSOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
    class TcpSplitterProcessor : public IProcessor
    {
        IRouter * router_;

    public:
        TcpSplitterProcessor(IRouter * router_ = NULL);
        virtual IProcessor * CreateCopy() const;

        virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset);
        virtual Protocol getProtocol();
        virtual void setRouter(IRouter * router_);
        virtual IRouter * router();
    };
    // class TcpSplitterProcessor
}
// namespace DiplomBukov

#endif // TCPSPLITTERPROCESSOR_H