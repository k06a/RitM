#ifndef IPV4PROCESSORCREATOR_H
#define IPV4PROCESSORCREATOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
    class Ipv4ProcessorCreator : public IProcessorCreator
    {
    public:
        virtual IProcessor * createProcessor(IRouter * router = NULL);
    };
    // class MacProcessorCreator
}
// namespace DiplomBukov

#endif // IPV4PROCESSORCREATOR_H