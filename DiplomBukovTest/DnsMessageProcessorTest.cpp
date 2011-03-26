#include "gmock\gmock.h"
#include "gtest\gtest.h"
#include "expect_file.h"

#include "CommonInclude.h"
#include "FileAdapter.h"
#include "MacHeaderProcessor.h"
#include "Ipv4HeaderProcessor.h"
#include "UdpHeaderProcessor.h"
#include "DnsMessageProcessor.h"
#include "AcceptProcessor.h"

using ::testing::_;
using ::testing::Return;

using namespace DiplomBukov;

//////////////////////////////////////////////////////////////////////////

TEST(DnsMessageProcessorTest, ReparseDnsHeader)
{
    {
        AdapterPtr adapter(
            new FileAdapter("DnsMessageProcessorTest.in.pcap",
                            "DnsMessageProcessorTest.out.pcap"));
        
        ProcessorPtr mac(new MacHeaderProcessor());
        ProcessorPtr ipv4Header(new Ipv4HeaderProcessor());
        ProcessorPtr udpHeader(new UdpHeaderProcessor());
        ProcessorPtr dnsProcessor(new DnsMessageProcessor());
        ProcessorPtr acceptProcessor(new AcceptProcessor());
        
        adapter->setSelf(adapter);
        mac->setSelf(mac);
        ipv4Header->setSelf(ipv4Header);
        udpHeader->setSelf(udpHeader);
        dnsProcessor->setSelf(dnsProcessor);
        acceptProcessor->setSelf(acceptProcessor);

        adapter->setNextProcessor(mac->getPointer());
        mac->setNextProcessor(ipv4Header->getPointer());
        ipv4Header->setNextProcessor(udpHeader->getPointer());
        udpHeader->setNextProcessor(dnsProcessor->getPointer());
        dnsProcessor->setNextProcessor(acceptProcessor->getPointer());

        adapter->ping(ProcessorPtr());

        adapter->run(true);

        adapter->DestroyHierarchy();
    }

    EXPECT_FILE_EQ("DnsMessageProcessorTest.in.pcap",
                   "DnsMessageProcessorTest.out.pcap");
}

//////////////////////////////////////////////////////////////////////////