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
    MOCK_CONST_METHOD0(CreateCopy, ProcessorPtr());
    MOCK_METHOD3(forwardProcess,ProcessingStatus(Protocol proto,
                                                 PacketPtr packet,
                                                 unsigned offset));
};

//////////////////////////////////////////////////////////////////////////

TEST(FileAdapterTest, ReadingWholePcapFile)
{
    AdapterPtr adapter(new FileAdapter("FileAdapterTest.pcap"));
    TestingReadingProcessor * testProc = new TestingReadingProcessor();
    ProcessorPtr processor(testProc);

    adapter->setSelf(adapter);
    processor->setSelf(processor);
    adapter->setNextProcessor(processor);
    adapter->ping(ProcessorPtr());

    EXPECT_CALL(*testProc, forwardProcess(_,_,_))
        .Times(60)
        .WillRepeatedly(Return(ProcessingStatus::Accepted));
    
    adapter->run(true);

    adapter->setSelf(AdapterPtr());
    processor->setSelf(ProcessorPtr());
    adapter->setNextProcessor(ProcessorPtr());
}

//////////////////////////////////////////////////////////////////////////

TEST(FileAdapterTest, ReadingSequentialyPcapFile)
{
    FileAdapterPtr adapter(new FileAdapter("FileAdapterTest.pcap"));
    TestingReadingProcessor * testProc = new TestingReadingProcessor();
    ProcessorPtr processor(testProc);

    adapter->setSelf(adapter);
    processor->setSelf(processor);
    adapter->setNextProcessor(processor);
    adapter->ping(ProcessorPtr());

    // --------------------------------

    adapter->run(false);

    EXPECT_CALL(*testProc, forwardProcess(_,_,_))
        .Times(60)
        .WillRepeatedly(Return(ProcessingStatus::Accepted));

    while (adapter->tick());

    // --------------------------------

    adapter->DestroyHierarchy();
}

//////////////////////////////////////////////////////////////////////////

TEST(FileAdapterTest, WritingPcapFile)
{
    // For smart pointer destruction
    {
        FileAdapterPtr adapter(
            new FileAdapter("FileAdapterTest.pcap",
                            "FileAdapterTest.WritingPcapFile.out.pcap"));
        AcceptProcessorPtr processor(new AcceptProcessor());

        adapter->setSelf(adapter);
        processor->setSelf(processor);

        adapter->setNextProcessor(processor);
        adapter->ping(ProcessorPtr());

        // --------------------------------

        adapter->run(true);

        // --------------------------------

        adapter->DestroyHierarchy();
    }

    EXPECT_FILE_EQ("FileAdapterTest.pcap",
                   "FileAdapterTest.WritingPcapFile.out.pcap");
}

//////////////////////////////////////////////////////////////////////////