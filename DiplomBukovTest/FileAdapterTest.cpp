#include "gtest\gtest.h"

#include "CommonInclude.h"
#include "FileAdapter.h"

using namespace DiplomBukov;

class TestingProcessor
    : public DiplomBukov::AbstractProcessor
{
public:
    int callCount;

    TestingProcessor()
        : callCount(0)
    {
    }

    IProcessorPtr CreateCopy() const
    {
        return IProcessorPtr(new TestingProcessor());
    }

    ProcessingStatus forwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
    {
        callCount++;
        return ProcessingStatus::Accepted;
    }
};

TEST(FileAdapterTest, ReadingPcapFile)
{
    IAdapterPtr adapter(new FileAdapter("FileAdapterTest.ReadingPcapFile.pcap"));
    IProcessorPtr processor(new TestingProcessor());

    adapter->setSelf(adapter);
    processor->setSelf(processor);

    adapter->setNextProcessor(processor);

    adapter->run(true);
}