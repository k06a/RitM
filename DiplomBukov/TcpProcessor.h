#ifndef TCPPROCESSOR_H
#define TCPPROCESSOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
    class TcpProcessor : public IProcessor
    {
        IRouter * router_;

    public:
        TcpProcessor(IRouter * router_ = NULL);
        virtual IProcessor * CreateCopy() const;

        virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset);
        virtual Protocol getProtocol();
        virtual void setRouter(IRouter * router_);
        virtual IRouter * router();
    };
    // class TcpProcessor
}
// namespace DiplomBukov

#endif // TCPPROCESSOR_H