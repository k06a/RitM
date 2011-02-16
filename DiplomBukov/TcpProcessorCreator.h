#ifndef TCPPROCESSORCREATOR_H
#define TCPPROCESSORCREATOR_H

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"
#include "IProcessorCreator.h"

namespace DiplomBukov
{
    class TcpProcessorCreator : public IProcessorCreator
    {
    public:
        virtual IProcessor * createProcessor(IRouter * router = NULL);
    };
    // class TcpProcessorCreator
}
// namespace DiplomBukov

#endif // TCPPROCESSORCREATOR_H