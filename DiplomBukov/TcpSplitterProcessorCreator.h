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
        virtual IProcessor * createProcessor(IRouter * router = NULL);
    };
    // class TcpSplitterProcessorCreator
}
// namespace DiplomBukov

#endif // TCPSPLITTERPROCESSORCREATOR_H