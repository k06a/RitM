#include "HttpDefragProcessor.h"
#include "IPacket.h"
#include <algorithm>

using namespace DiplomBukov;

HttpDefragProcessor::HttpDefragProcessor(ProcessorPtr processor)
    : markCount(0), positionOfData(0)
{
    setNextProcessor(processor);
}

ProcessorPtr HttpDefragProcessor::CreateCopy() const
{
    ProcessorPtr ptr(new HttpDefragProcessor(nextProcessor->CreateCopy()));
    ptr->setSelf(ptr);
    return ptr;
}

template<typename T>
bool startsWith(const T & vec, const std::string & str)
{
    for (unsigned i = 0; i < str.size(); i++)
        if (vec[i] != str[i]) return false;
    return true;
}

// RFC2616 Section 3.6.1
bool HttpDefragProcessor::checkEofChunkedData(const Blob & blob, int positionOfData)
{
    std::string str((char*)&blob[positionOfData], blob.size()-positionOfData);
    std::istringstream stream(str);

    int chunkCount = 0;
    while (true)
    {
        // Get hexed chunk size
        unsigned chunkSize = 0;
        std::string chunkSizeAndExtension;
        int pos = stream.tellg();
        std::getline(stream, chunkSizeAndExtension, '\n');
        int ret = sscanf_s(&chunkSizeAndExtension[0], "%x", &chunkSize);
        if (ret != 1)
        {
            stream.seekg(pos);
            break;
        }
        chunkCount++;

        // Skip chunk data
        stream.seekg((int)stream.gcount() + chunkSize);
        if (stream.eof()) return false;
    }

    if (chunkCount == 0)
        return false;
    
    std::string line;
    while (!stream.eof())
        std::getline(stream, line, '\n');
    
    return (line.size() == 0);
}

ProcessingStatus HttpDefragProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(Self);

    // Empty packet
    if (offset == packet->size())
        return ProcessingStatus::Accepted;

    int oldSize = blob.size();
    blob.resize(oldSize + packet->size() - offset);
    std::copy(
        packet->data().begin() + offset,
        packet->data().end(),
        blob.begin() + oldSize);

    // Checking end of chunk
    

    if (oldSize == 0)   // Read options only first time
    {
        httpHeader.options.clear();

        std::string str((char*)&blob[0], blob.size());
        std::istringstream stream(str);
        std::getline(stream, httpHeader.firstLine, '\n');

        if (startsWith(httpHeader.firstLine, "GET"))
        {
            httpHeader.type = HttpHeader::GetRequest;
            httpHeader.operation = "GET";
        } else
        if (startsWith(httpHeader.firstLine, "POST"))
        {
            httpHeader.type = HttpHeader::PostRequest;
            httpHeader.operation = "POST";
        } else
        if (startsWith(httpHeader.firstLine, "HTTP/"))
        {
            httpHeader.type = HttpHeader::HttpAnswer;
            httpHeader.operation = "HTTP";
            httpHeader.code = 0;
            sscanf_s(&httpHeader.firstLine[9], "%d", &httpHeader.code);
        }
        else
        {
            backwardProcess(proto, packet, offset);
        }

        while (true)
        {
            std::string line;
            std::string optionName;
            std::string optionValue;

            std::getline(stream, line, '\n');
            if (line.size() < 2) break;
            line.resize(line.size() - 1);
            
            optionName = line.substr(0, line.find(':'));
            optionValue = line.substr(optionName.size()+2);

            httpHeader.options.push_back(std::make_pair(optionName, optionValue));
        }
        positionOfData = stream.gcount();
    }

    if (httpHeader.type == HttpHeader::GetRequest)
    {
        packet->data() = blob;
        packet->setRealSize(packet->data().size());
        backwardProcess(proto, packet, 0);
        return ProcessingStatus::Accepted;
    }

    // Length determine from RFC2616 Section 4.4
    bool fullPacket = false;

    // RFC2616 Section 4.4.1
    if ((httpHeader.type == HttpHeader::HttpAnswer) &&
        ((httpHeader.code >= 100) && (httpHeader.code <= 199) ||
         (httpHeader.code == 204) || (httpHeader.code <= 304)))
    {
        const char rnrn[] = "\r\n\r\n";
        Blob::iterator it = std::search(blob.begin(), blob.end(), rnrn, rnrn + 4);
        fullPacket = (it != blob.end());
    }

    // RFC2616 Section 4.4.2
    if (httpHeader.type == HttpHeader::HttpAnswer)
    {
        for(HttpOptionList::iterator opt = httpHeader.options.begin();
            opt != httpHeader.options.end(); ++opt)
        {
            if (opt->first == "Transfer-Encoding")
            {
                if (opt->second == "chunked")
                    fullPacket = checkEofChunkedData(blob, positionOfData);
                else
                    break;
            }
        }
    }
    
    return ProcessingStatus::Accepted;

    //packet->addProcessor(Self);
    //if (nextProcessor != NULL)
    //    nextProcessor->forwardProcess("HTTP", packet, 0);
}

ProcessingStatus HttpDefragProcessor::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if (packet->processorBefore(Self) != NULL)
        packet->processorBefore(Self)->backwardProcess(proto, packet, offset);
    return ProcessingStatus::Accepted;
}

Protocol HttpDefragProcessor::getProtocol()
{
    return Protocol("TCP_80", 80);
}

const char * HttpDefragProcessor::getProcessorName()
{
    return "HttpDefragProcessor";
}