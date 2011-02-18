#ifndef ICMPPROCESSOR_H
#define ICMPPROCESSOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
    class IcmpProcessor : public IProcessor
    {
        IProcessorModule * module;
        IRouter * baseRouter;
        
    public:
        IcmpProcessor(IRouter * router = NULL);
        virtual IProcessor * CreateCopy() const;

        virtual ProcessingStatus processPacket(Protocol proto, Packet & packet, unsigned offset);
        virtual Protocol getProtocol();
        virtual const char * getProcessorName();

        virtual void setRouter(IRouter * router);
        virtual IRouter * getRouter();

        virtual void setModule(IProcessorModule * module);
        virtual IProcessorModule * getModule();
    };
    // class IcmpProcessor
}
// namespace DiplomBukov

#endif // ICMPPROCESSOR_H