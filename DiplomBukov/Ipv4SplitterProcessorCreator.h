#ifndef Ipv4SplitterProcessorCREATOR_H
#define Ipv4SplitterProcessorCREATOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
    class Ipv4SplitterProcessorCreator : public IProcessorCreator
    {
    public:
        virtual IProcessor * createProcessor(IRouter * router = NULL);
    };
    // class MacProcessorCreator
}
// namespace DiplomBukov

#endif // Ipv4SplitterProcessorCREATOR_H