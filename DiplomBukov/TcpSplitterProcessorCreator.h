#ifndef TCPSPLITTERPROCESSORCREATOR_H
#define TCPSPLITTERPROCESSORCREATOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
    class TcpSplitterProcessorCreator : public IProcessorCreator
    {
    public:
        virtual IProcessorPtr createProcessor(IRouterPtr router = IRouterPtr()) const;
    };
    // class TcpSplitterProcessorCreator
}
// namespace DiplomBukov

#endif // TCPSPLITTERPROCESSORCREATOR_H