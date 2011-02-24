#ifndef Ipv4SplitterCREATOR_H
#define Ipv4SplitterCREATOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
    class Ipv4SplitterCreator : public IProcessorCreator
    {
    public:
        virtual IProcessorPtr createProcessor(IRouterPtr router = IRouterPtr()) const;
    };
    // class MacProcessorCreator
}
// namespace DiplomBukov

#endif // Ipv4SplitterCREATOR_H