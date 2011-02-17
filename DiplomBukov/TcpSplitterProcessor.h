#ifndef TCPSPLITTERPROCESSOR_H
#define TCPSPLITTERPROCESSOR_H

#include <map>
#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
    class TcpSplitterProcessor : public IProcessor
    {
        typedef std::pair<unsigned short,unsigned short> port_pair;
        typedef std::map<port_pair,IRouter*> MyMap;

        IRouter * baseRouter;
        MyMap routers;

    public:
        TcpSplitterProcessor(IRouter * router = NULL);
        virtual IProcessor * CreateCopy() const;

        virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset);
        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
        virtual void setRouter(IRouter * router);
        virtual IRouter * router();
    };
    // class TcpSplitterProcessor
}
// namespace DiplomBukov

#endif // TCPSPLITTERPROCESSOR_H