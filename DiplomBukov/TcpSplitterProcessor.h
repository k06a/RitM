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

        IProcessorModule * module;
        IRouter * router;
        MyMap routers;

    public:
        TcpSplitterProcessor(IRouter * router = NULL);
        virtual IProcessor * CreateCopy() const;

        virtual IPacketProcessor * getPointer();
        virtual void ping(IPacketProcessor * prevProcessor);
        virtual ProcessingStatus forwardProcess(Protocol proto, Packet & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, Packet & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();

        virtual void setRouter(IRouter * router);
        virtual IRouter * getRouter();

        virtual void setModule(IProcessorModule * module);
        virtual IProcessorModule * getModule();
    };
    // class TcpSplitterProcessor
}
// namespace DiplomBukov

#endif // TCPSPLITTERPROCESSOR_H