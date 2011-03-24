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
        HttpSwapProcessor(IProcessorPtr processor = IProcessorPtr());
        virtual IProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();
    };
    // class HttpSwapProcessor
}
// namespace DiplomBukov

#endif // HTTPSWAPPROCESSOR_H