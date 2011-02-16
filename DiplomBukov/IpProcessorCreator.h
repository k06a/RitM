#ifndef IPPROCESSORCREATOR_H
#define IPPROCESSORCREATOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
    class IpProcessorCreator : public IProcessorCreator
    {
    public:
        virtual IProcessor * createProcessor(IRouter * router = NULL);
    };
    // class MacProcessorCreator
}
// namespace DiplomBukov

#endif // IPPROCESSORCREATOR_H