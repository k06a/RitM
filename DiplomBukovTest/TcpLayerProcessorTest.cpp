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
        IAdapterPtr adapter1(
            new FileAdapter("TcpLayerProcessorTest.clientSide.in.pcap",
            "TcpLayerProcessorTest.serverSide.out.pcap"));
        IAdapterPtr adapter2(
            new FileAdapter("TcpLayerProcessorTest.serverSide.in.pcap",
            "TcpLayerProcessorTest.clientSide.out.pcap"));

        IProcessorPtr mac1(new MacHeaderProcessor());
        IProcessorPtr mac2(new MacHeaderProcessor());
        IProcessorPtr macSwitch(new MacSwitch());
        IProcessorPtr ipv4Splitter(new Ipv4Splitter());
        IProcessorPtr ipv4Header(new Ipv4HeaderProcessor());
        IProcessorPtr tcpProcessor(new TcpLayerProcessor());
        IProcessorPtr acceptProcessor(new AcceptProcessor());
        
        adapter1->setSelf(adapter1);
        adapter2->setSelf(adapter2);
        mac1->setSelf(mac1);
        mac2->setSelf(mac2);
        macSwitch->setSelf(macSwitch);
        ipv4Splitter->setSelf(ipv4Splitter);
        ipv4Header->setSelf(ipv4Header);
        tcpProcessor->setSelf(tcpProcessor);
        acceptProcessor->setSelf(acceptProcessor);

        adapter1->setNextProcessor(mac1->getPointer());
        adapter2->setNextProcessor(mac2->getPointer());
        mac1->setNextProcessor(macSwitch->getPointer());
        mac2->setNextProcessor(macSwitch->getPointer());
        macSwitch->setNextProcessor(ipv4Splitter->getPointer());
        ipv4Splitter->setNextProcessor(ipv4Header->getPointer());
        ipv4Header->setNextProcessor(tcpProcessor->getPointer());
        tcpProcessor->setNextProcessor(acceptProcessor->getPointer());

        adapter1->ping(IProcessorPtr());
        adapter2->ping(IProcessorPtr());

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

    EXPECT_FILE_EQ("TcpLayerProcessorTest.clientSide.in.pcap",
                   "TcpLayerProcessorTest.serverSide.out.pcap");
    EXPECT_FILE_EQ("TcpLayerProcessorTest.serverSide.in.pcap",
                   "TcpLayerProcessorTest.clientSide.out.pcap");
}

//////////////////////////////////////////////////////////////////////////