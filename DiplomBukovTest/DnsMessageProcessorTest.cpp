#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "expect_file.h"

#include "FileAdapter.h"
#include "MacHeaderProcessor.h"
#include "Ipv4HeaderProcessor.h"
#include "UdpHeaderProcessor.h"
#include "AcceptProcessor.h"

using ::testing::_;
using ::testing::Return;

using namespace DiplomBukov;

//////////////////////////////////////////////////////////////////////////

TEST(DnsMessageProcessorTest, AnswersHijacking)
{
    {
        IAdapterPtr adapter(
            new FileAdapter("DnsMessageProcessorTest.in.pcap",
                            "DnsMessageProcessorTest.out.pcap"));
        
        IProcessorPtr mac(new MacHeaderProcessor());
        IProcessorPtr ipv4Header(new Ipv4HeaderProcessor());
        IProcessorPtr UdpHeader(new UdpHeaderProcessor());
        IProcessorPtr acceptProcessor(new AcceptProcessor());
        
        adapter->setSelf(adapter);
        mac->setSelf(mac);
        ipv4Header->setSelf(ipv4Header);
        UdpHeader->setSelf(UdpHeader);
        acceptProcessor->setSelf(acceptProcessor);

        adapter->setNextProcessor(mac->getPointer());
        mac->setNextProcessor(ipv4Header->getPointer());
        ipv4Header->setNextProcessor(UdpHeader->getPointer());
        UdpHeader->setNextProcessor(acceptProcessor->getPointer());

        adapter->ping(IProcessorPtr());

        adapter->run(true);

        adapter->DestroyHierarchy();
    }

    EXPECT_FILE_EQ("DnsMessageProcessorTest.good_out.pcap",
                   "DnsMessageProcessorTest.out.pcap");
}

//////////////////////////////////////////////////////////////////////////