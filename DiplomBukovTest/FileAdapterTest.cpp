#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "expect_file.h"

#include "FileAdapter.h"
#include "AcceptProcessor.h"

using ::testing::_;
using ::testing::Return;

using namespace DiplomBukov;

//////////////////////////////////////////////////////////////////////////

class TestingReadingProcessor
    : public DiplomBukov::AbstractProcessor
{
public:
    MOCK_CONST_METHOD0(CreateCopy, IProcessorPtr());
    MOCK_METHOD3(forwardProcess,ProcessingStatus(Protocol proto,
                                                 IPacketPtr packet,
                                                 unsigned offset));
};

//////////////////////////////////////////////////////////////////////////

TEST(FileAdapterTest, ReadingWholePcapFile)
{
    IAdapterPtr adapter(new FileAdapter("FileAdapterTest.pcap"));
    TestingReadingProcessor * testProc = new TestingReadingProcessor();
    IProcessorPtr processor(testProc);

    adapter->setSelf(adapter);
    processor->setSelf(processor);
    adapter->setNextProcessor(processor);
    adapter->ping(IProcessorPtr());

    EXPECT_CALL(*testProc, forwardProcess(_,_,_))
        .Times(60)
        .WillRepeatedly(Return(ProcessingStatus::Accepted));
    
    adapter->run(true);

    adapter->setSelf(IAdapterPtr());
    processor->setSelf(IProcessorPtr());
    adapter->setNextProcessor(IProcessorPtr());
}

//////////////////////////////////////////////////////////////////////////

TEST(FileAdapterTest, ReadingSequentialyPcapFile)
{
    IAdapterPtr adapter(new FileAdapter("FileAdapterTest.pcap"));
    TestingReadingProcessor * testProc = new TestingReadingProcessor();
    IProcessorPtr processor(testProc);

    adapter->setSelf(adapter);
    processor->setSelf(processor);
    adapter->setNextProcessor(processor);
    adapter->ping(IProcessorPtr());

    adapter->run(false);

    EXPECT_CALL(*testProc, forwardProcess(_,_,_))
        .Times(60)
        .WillRepeatedly(Return(ProcessingStatus::Accepted));

    while (adapter->tick());

    adapter->setSelf(IAdapterPtr());
    processor->setSelf(IProcessorPtr());
    adapter->setNextProcessor(IProcessorPtr());
}

//////////////////////////////////////////////////////////////////////////

TEST(FileAdapterTest, WritingPcapFile)
{
    // For smart pointer destruction
    {
        IAdapterPtr adapter(
            new FileAdapter("FileAdapterTest.pcap",
                            "FileAdapterTest.WritingPcapFile.out.pcap"));
        IProcessorPtr processor(new AcceptProcessor());

        adapter->setSelf(adapter);
        processor->setSelf(processor);
        adapter->setNextProcessor(processor);
        adapter->ping(IProcessorPtr());

        adapter->run(true);

        adapter->setSelf(IAdapterPtr());
        processor->setSelf(IProcessorPtr());
        adapter->setNextProcessor(IProcessorPtr());
    }

    EXPECT_FILE_EQ("FileAdapterTest.pcap",
                   "FileAdapterTest.WritingPcapFile.out.pcap");
}

//////////////////////////////////////////////////////////////////////////