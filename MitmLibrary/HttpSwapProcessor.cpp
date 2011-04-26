#include "HttpSwapProcessor.h"
#include "IPacket.h"
#include <algorithm>

using namespace DiplomBukov;

HttpSwapProcessor::HttpSwapProcessor(ProcessorPtr processor)
: markCount(0), positionOfData(0)
{
    setNextProcessor(processor);
}

ProcessorPtr HttpSwapProcessor::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new HttpSwapProcessor(np));
}

ProcessingStatus HttpSwapProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    //if ((proto != Protocol::None) && (proto != getProtocol()))
    //    return ProcessingStatus::Rejected;

    // Empty packet
    if (offset == packet->size())
        return ProcessingStatus::Accepted;

    int oldSize = blob.size();
    blob.resize(oldSize + packet->size() - offset);
    std::copy(
        packet->dataBegin() + offset,
        packet->dataEnd(),
        blob.begin() + oldSize);

    // Checking end of chunk
    enum MessageType
    {
        GetRequest  = 1,
        PostRequest = 2,
        HttpAnswer  = 4
    };

    int type = 0;
    type |= GetRequest  * (blob[0] == 'G');
    type |= PostRequest * (blob[0] == 'P');
    type |= HttpAnswer  * (blob[0] == 'H');

    if (oldSize == 0)   // Read options only one time
    {
        httpOptions.clear();
        std::string firstLine;

        std::string str((char*)&blob[0], blob.size());
        std::istringstream stream(str);
        std::getline(stream, firstLine, '\n');
        while (true)
        {
            std::string line;
            std::string optionName;
            std::string optionValue;
            std::getline(stream, line, '\n');
            line.resize(line.size() - 1);
            if (line.size() == 0)
                break;

            optionName = line.substr(0, line.find(':'));
            optionValue = line.substr(optionName.size()+2);

            httpOptions.push_back(std::make_pair(optionName, optionValue));
        }
        positionOfData = stream.tellg();
    }

    switch (type)
    {
    case GetRequest:
        {
            int a = 0;
            a = 1;
        }
        break;
    case PostRequest:
        {
            int a = 0;
            a = 1;
        }
        break;
    case HttpAnswer:
        {

            int a = 0;
            a = 1;
        }
        break;
    }



    return ProcessingStatus::Accepted;

    //packet->addProcessor(Self);
    //if (nextProcessor != NULL)
    //    nextProcessor->forwardProcess("HTTP", packet, 0);
}

ProcessingStatus HttpSwapProcessor::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    return ProcessingStatus::Accepted;
}

Protocol HttpSwapProcessor::getProtocol()
{
    return Protocol::None;//Protocol("TCP_80", 80);
}

const char * HttpSwapProcessor::getProcessorName()
{
    return "HttpSwapProcessor";
}