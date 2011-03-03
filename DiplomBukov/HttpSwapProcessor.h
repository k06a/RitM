#ifndef HTTPSWAPPROCESSOR_H
#define HTTPSWAPPROCESSOR_H

#include <deque>

#include "CommonInclude.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
    class HttpSwapProcessor : public AbstractProcessor
    {
        typedef std::deque<u8> MyDeque;

        MyDeque blob;
        int markCount;

    public:
        HttpSwapProcessor(IProcessorPtr processor = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class HttpSwapProcessor
}
// namespace DiplomBukov

#endif // HTTPSWAPPROCESSOR_H