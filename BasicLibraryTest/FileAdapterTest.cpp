#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "expect_file.h"

#include "FileAdapter.h"
#include "AcceptProcessor.h"

using ::testing::_;
using ::testing::Return;

using namespace RitM;

//////////////////////////////////////////////////////////////////////////

class TestingReadingProcessor
    : public RitM::AbstractProcessor
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
    AdapterPtr adapter(new FileAdapter());
    GroupOptionPtr gr = SharedPointerCast<GroupOption>(adapter->getOptions());
    FileOpenOptionPtr infile = SharedPointerCast<FileOpenOption>(gr->options_item(0));
    infile->setFilename("FileAdapterTest.pcap");

    TestingReadingProcessor * testProc = new TestingReadingProcessor();
    ProcessorPtr processor(testProc);

    adapter->setNextProcessor(processor);
    adapter->ping(ProcessorPtr());

    EXPECT_CALL(*testProc, forwardProcess(_,_,_))
        .Times(60)
        .WillRepeatedly(Return(ProcessingStatus::Accepted));
    
    adapter->run(true);

    adapter->DestroyHierarchy();
}

//////////////////////////////////////////////////////////////////////////

TEST(FileAdapterTest, ReadingSequentialyPcapFile)
{
    AdapterPtr adapter(new FileAdapter());
    GroupOptionPtr gr = SharedPointerCast<GroupOption>(adapter->getOptions());
    FileOpenOptionPtr infile = SharedPointerCast<FileOpenOption>(gr->options_item(0));
    infile->setFilename("FileAdapterTest.pcap");

    TestingReadingProcessor * testProc = new TestingReadingProcessor();
    ProcessorPtr processor(testProc);

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
        AdapterPtr adapter(new FileAdapter());
        GroupOptionPtr gr = SharedPointerCast<GroupOption>(adapter->getOptions());
        FileOpenOptionPtr infile  = SharedPointerCast<FileOpenOption>(gr->options_item(0));
        FileOpenOptionPtr outfile = SharedPointerCast<FileOpenOption>(gr->options_item(1));
        infile->setFilename("FileAdapterTest.pcap");
        outfile->setFilename("FileAdapterTest.WritingPcapFile.out.pcap");

        AcceptProcessorPtr processor(new AcceptProcessor());

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