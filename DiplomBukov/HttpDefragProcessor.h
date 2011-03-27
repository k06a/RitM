#ifndef HTTPDEFRAGPROCESSOR_H
#define HTTPDEFRAGPROCESSOR_H

#include <string>
#include <vector>
#include <utility>
#include <sstream>

#include "CommonInclude.h"
#include "AbstractProcessor.h"

namespace DiplomBukov
{
    class HttpDefragProcessor : public AbstractProcessor
    {
        typedef std::vector<u8> Blob;
        typedef std::pair<std::string,std::string> HttpOption;
        typedef std::vector<HttpOption> HttpOptionList;

        struct HttpHeader
        {
            enum MessageType
            {
                GetRequest  = 1,
                PostRequest = 2,
                HttpAnswer  = 4
            };

            std::string firstLine;
            std::string operation;  // HTTP, GET, POST
            std::string url;        // /html/1.jpg
            std::string version;    // HTTP/1.1
            int         code;       // 200
            MessageType type;

            HttpOptionList options;
        };

        Blob blob;
        unsigned positionOfData;
        HttpHeader httpHeader;
        int markCount;

    public:
        HttpDefragProcessor(ProcessorPtr processor = ProcessorPtr());
        virtual ProcessorPtr CreateCopy() const;

        virtual ProcessingStatus forwardProcess(Protocol proto, PacketPtr packet, unsigned offset);
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual Protocol getProtocol();
        virtual const char * getProcessorName();

        bool checkEofChunkedData(const Blob & blob, int positionOfData);
    };
    // class HttpDefragProcessor

    typedef SmartPointer<HttpDefragProcessor>::Type HttpDefragProcessorPtr;
}
// namespace DiplomBukov

#endif // HTTPDEFRAGPROCESSOR_H