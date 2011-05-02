#ifndef HTTPSWAPPROCESSOR_H
#define HTTPSWAPPROCESSOR_H

#include <string>
#include <vector>
#include <utility>
#include <sstream>

#include "CommonInclude.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
    class HttpSwapProcessor : public AbstractProcessor
    {
        typedef std::vector<u8> Blob;
        typedef std::vector<std::pair<std::string,std::string> > OptionList;

        Blob blob;
        unsigned positionOfData;
        OptionList httpOptions;
        int markCount;

    public:
        HttpSwapProcessor(ProcessorPtr processor = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();

        virtual bool isFinal();
    };
    // class HttpSwapProcessor

    typedef SharedPointer<HttpSwapProcessor>::Type HttpSwapProcessorPtr;
}
// namespace DiplomBukov

#endif // HTTPSWAPPROCESSOR_H