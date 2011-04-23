#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "expect_file.h"

#include "FileAdapter.h"
#include "MacHeaderProcessor.h"
#include "MacSwitch.h"
#include "TcpLayerProcessor.h"
#include "FileAdapterPairReader.h"
#include "Ipv4HeaderProcessor.h"
#include "TcpHeaderProcessor.h"
#include "Ipv4Splitter.h"
#include "TcpSplitter.h"
#include "AcceptProcessor.h"

using ::testing::_;
using ::testing::Return;

using namespace DiplomBukov;

//////////////////////////////////////////////////////////////////////////

TEST(TcpLayerProcessorTest, ConnectionEstablishing)
{
    {
        AdapterPtr adapter1(new FileAdapter());
        GroupOptionPtr gr1 = SharedPointerCast<GroupOption>(adapter1->getOptions());
        FileOpenOptionPtr infile1  = SharedPointerCast<FileOpenOption>(gr1->options()[0]);
        FileOpenOptionPtr outfile1 = SharedPointerCast<FileOpenOption>(gr1->options()[1]);
        infile1->setFilename("TcpLayerProcessorTest.clientSide.in.pcap");
        outfile1->setFilename("TcpLayerProcessorTest.clientSide.out.pcap");

        AdapterPtr adapter2(new FileAdapter());
        GroupOptionPtr gr2 = SharedPointerCast<GroupOption>(adapter2->getOptions());
        FileOpenOptionPtr infile2  = SharedPointerCast<FileOpenOption>(gr2->options()[0]);
        FileOpenOptionPtr outfile2 = SharedPointerCast<FileOpenOption>(gr2->options()[1]);
        infile2->setFilename("TcpLayerProcessorTest.serverSide.in.pcap");
        outfile2->setFilename("TcpLayerProcessorTest.serverSide.out.pcap");

        MacHeaderProcessorPtr mac1(new MacHeaderProcessor());
        MacHeaderProcessorPtr mac2(new MacHeaderProcessor());
        MacSwitchPtr macSwitch(new MacSwitch());
        Ipv4SplitterPtr ipv4Splitter(new Ipv4Splitter());
        Ipv4HeaderProcessorPtr ipv4Header(new Ipv4HeaderProcessor());
        TcpSplitterPtr tcpSplitter(new TcpSplitter());
        TcpLayerProcessorPtr tcpProcessor(new TcpLayerProcessor());
        TcpHeaderProcessorPtr tcpHeader(new TcpHeaderProcessor());
        AcceptProcessorPtr acceptProcessor(new AcceptProcessor());
        
        adapter1->setNextProcessor(mac1->getPointer());
        adapter2->setNextProcessor(mac2->getPointer());
        mac1->setNextProcessor(macSwitch->getPointer());
        mac2->setNextProcessor(macSwitch->getPointer());
        macSwitch->setNextProcessor(ipv4Splitter->getPointer());
        ipv4Splitter->setNextProcessor(ipv4Header->getPointer());
        ipv4Header->setNextProcessor(tcpSplitter->getPointer());
        tcpSplitter->setNextProcessor(tcpProcessor->getPointer());
        tcpProcessor->setNextProcessor(tcpHeader->getPointer());
        tcpHeader->setNextProcessor(acceptProcessor->getPointer());
        adapter1->ping(ProcessorPtr());
        adapter2->ping(ProcessorPtr());

        // --------------------------------

        adapter1->run(false);
        adapter2->run(false);
        fileAdapterPairReader(adapter1, adapter2);

        // --------------------------------

        adapter1->DestroyHierarchy();
        adapter2->DestroyHierarchy();
    }

    EXPECT_FILE_EQ("TcpLayerProcessorTest.clientSide.good_out.pcap",
                   "TcpLayerProcessorTest.clientSide.out.pcap");
    EXPECT_FILE_EQ("TcpLayerProcessorTest.serverSide.good_out.pcap",
                   "TcpLayerProcessorTest.serverSide.out.pcap");
}

//////////////////////////////////////////////////////////////////////////