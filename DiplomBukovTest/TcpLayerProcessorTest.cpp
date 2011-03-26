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
        AdapterPtr adapter1(
            new FileAdapter("TcpLayerProcessorTest.clientSide.in.pcap",
                            "TcpLayerProcessorTest.clientSide.out.pcap"));
        AdapterPtr adapter2(
            new FileAdapter("TcpLayerProcessorTest.serverSide.in.pcap",
                            "TcpLayerProcessorTest.serverSide.out.pcap"));

        ProcessorPtr mac1(new MacHeaderProcessor());
        ProcessorPtr mac2(new MacHeaderProcessor());
        ProcessorPtr macSwitch(new MacSwitch());
        ProcessorPtr ipv4Splitter(new Ipv4Splitter());
        ProcessorPtr ipv4Header(new Ipv4HeaderProcessor());
        ProcessorPtr tcpSplitter(new TcpSplitter());
        ProcessorPtr tcpProcessor(new TcpLayerProcessor());
        ProcessorPtr tcpHeader(new TcpHeaderProcessor());
        ProcessorPtr acceptProcessor(new AcceptProcessor());
        
        adapter1->setSelf(adapter1);
        adapter2->setSelf(adapter2);
        mac1->setSelf(mac1);
        mac2->setSelf(mac2);
        macSwitch->setSelf(macSwitch);
        ipv4Splitter->setSelf(ipv4Splitter);
        ipv4Header->setSelf(ipv4Header);
        tcpSplitter->setSelf(tcpSplitter);
        tcpProcessor->setSelf(tcpProcessor);
        tcpHeader->setSelf(tcpHeader);
        acceptProcessor->setSelf(acceptProcessor);

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

/*
        bool b1 = true;
        bool b2 = true;
        while (b1 && b2)
        {
            b1 = adapter1->tick();
            b2 = adapter2->tick();
        }
*/
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