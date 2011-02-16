#ifndef MACPROCESSORCREATOR_H
#define MACPROCESSORCREATOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
    class MacProcessorCreator : public IProcessorCreator
    {
    public:
        virtual IProcessor * createProcessor(IRouter * router = NULL);
    };
    // class MacProcessorCreator
}
// namespace DiplomBukov

#endif // MACPROCESSORCREATOR_H