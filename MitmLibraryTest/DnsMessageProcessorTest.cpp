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

#include "CheckOption.h"
#include "SwitchOption.h"
#include "TextLineOption.h"
#include "GroupOption.h"

using ::testing::_;
using ::testing::Return;

using namespace RitM;

//////////////////////////////////////////////////////////////////////////

TEST(DnsMessageProcessorTest, ReparseDnsHeader)
{
    {
        AdapterPtr adapter(new FileAdapter());
        GroupOptionPtr gr = SharedPointerCast<GroupOption>(adapter->getOptions());
        FileOpenOptionPtr infile  = SharedPointerCast<FileOpenOption>(gr->options_item(0));
        FileOpenOptionPtr outfile = SharedPointerCast<FileOpenOption>(gr->options_item(1));
        infile->setFilename("DnsMessageProcessorTest.in.pcap");
        outfile->setFilename("DnsMessageProcessorTest.ReparseDnsHeader.out.pcap");
        
        MacHeaderProcessorPtr mac(new MacHeaderProcessor());
        Ipv4HeaderProcessorPtr ipv4Header(new Ipv4HeaderProcessor());
        UdpHeaderProcessorPtr udpHeader(new UdpHeaderProcessor());
        DnsMessageProcessorPtr dnsProcessor(new DnsMessageProcessor());
        AcceptProcessorPtr acceptProcessor(new AcceptProcessor());
        
        adapter->setNextProcessor(mac->getPointer());
        mac->setNextProcessor(ipv4Header->getPointer());
        ipv4Header->setNextProcessor(udpHeader->getPointer());
        udpHeader->setNextProcessor(dnsProcessor->getPointer());
        dnsProcessor->setNextProcessor(acceptProcessor->getPointer());
        adapter->ping(ProcessorPtr());

        GroupOption * opts = (GroupOption *)dnsProcessor->getOptions().get();
        CheckOption * always = (CheckOption *)opts->options_item(0).get();
        always->setChecked(true);

        adapter->run(true);

        adapter->DestroyHierarchy();
    }

    EXPECT_FILE_EQ("DnsMessageProcessorTest.in.pcap",
                   "DnsMessageProcessorTest.ReparseDnsHeader.out.pcap");
}

//////////////////////////////////////////////////////////////////////////

TEST(DnsMessageProcessorTest, SwapNetbsdToYandex)
{
    {
        AdapterPtr adapter(new FileAdapter());
        GroupOptionPtr gr = SharedPointerCast<GroupOption>(adapter->getOptions());
        FileOpenOptionPtr infile  = SharedPointerCast<FileOpenOption>(gr->options_item(0));
        FileOpenOptionPtr outfile = SharedPointerCast<FileOpenOption>(gr->options_item(1));
        infile->setFilename("DnsMessageProcessorTest.in.pcap");
        outfile->setFilename("DnsMessageProcessorTest.SwapNetbsdToYandex.out.pcap");
        
        MacHeaderProcessorPtr mac(new MacHeaderProcessor());
        Ipv4HeaderProcessorPtr ipv4Header(new Ipv4HeaderProcessor());
        UdpHeaderProcessorPtr udpHeader(new UdpHeaderProcessor());
        DnsMessageProcessorPtr dnsProcessor(new DnsMessageProcessor());
        AcceptProcessorPtr acceptProcessor(new AcceptProcessor());

        adapter->setNextProcessor(mac->getPointer());
        mac->setNextProcessor(ipv4Header->getPointer());
        ipv4Header->setNextProcessor(udpHeader->getPointer());
        udpHeader->setNextProcessor(dnsProcessor->getPointer());
        dnsProcessor->setNextProcessor(acceptProcessor->getPointer());
        adapter->ping(ProcessorPtr());

        GroupOption * opts = (GroupOption *)dnsProcessor->getOptions().get();
        GroupOption * dnsOpt = (GroupOption *)opts->options_item(1).get();
        CheckOption * checked = (CheckOption *)dnsOpt->options_item(0).get();
        TextLineOption * source = (TextLineOption *)dnsOpt->options_item(1).get();
        SwitchOption * destType = (SwitchOption *)dnsOpt->options_item(2).get();
        TextLineOption * destination = (TextLineOption *)dnsOpt->options_item(3).get();

        checked->setChecked(true);
        source->setText("www.netbsd.org");
        destType->setSelectedIndex(destType->getIndexOf("IPv4"));
        destination->setText("77.88.21.11");

        // --------------------------------

        adapter->run(true);
        
        // --------------------------------

        adapter->DestroyHierarchy();
    }

    EXPECT_FILE_EQ("DnsMessageProcessorTest.SwapNetbsdToYandex.good_out.pcap",
                   "DnsMessageProcessorTest.SwapNetbsdToYandex.out.pcap");
}

//////////////////////////////////////////////////////////////////////////