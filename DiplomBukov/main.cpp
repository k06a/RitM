#include <deque>
#include <string>
#include <iostream>

#include "IConnector.h"
#include "IAdapter.h"
#include "IProcessor.h"

#include "ProcessorModule.h"

#include "FileAdapter.h"
#include "PcapAdapter.h"
#include "BruteConnector.h"
#include "ProtocolConnector.h"

#include "AcceptProcessor.h"
#include "RejectProcessor.h"

#include "MacHeaderProcessor.h"
#include "Ipv4HeaderProcessor.h"
#include "IcmpProcessor.h"
#include "TcpOptionsRemover.h"
#include "TcpLayerProcessor.h"
#include "TcpHeaderProcessor.h"

#include "TelnetSwapper.h"
#include "HttpDefragProcessor.h"

#include "Ipv4Splitter.h"
#include "Ipv4Defragger.h"
#include "TcpSplitter.h"
#include "UdpSplitter.h"

#include "MacSwitch.h"

#include "FileAdapterPairReader.h"

using namespace DiplomBukov;

void print_arch(IProcessorPtr proc, std::string prefix = "", int deep = 0)
{
    if (proc == NULL) return;

    IProcessor * ptr = proc.get();
    IConnector * ic = dynamic_cast<IConnector*>(ptr);
    IAdapter   * ia = dynamic_cast<IAdapter*>(ptr);
    IProcessor * ip = dynamic_cast<IProcessor*>(ptr);

    if (ic != NULL)
    {
        if (deep == 1)
        std::cout << " [ Connector"
            << '{' << dynamic_cast<IAdapter*>(ic->getPrevProcessor().get())
            << ',' << (IProcessor *)ic
            << ',' << (IProcessor *)ic->getNextProcessor().get() << '}'
            << " ]:";
        else
        std::cout << " [ Connector"
            << '{' << (IProcessor *)ic->getPrevProcessor().get()
            << ',' << (IProcessor *)ic
            << ',' << (IProcessor *)ic->getNextProcessor().get() << '}'
            << " ]:";
        const std::deque<IProcessorPtr> & procList = ic->nextProcessors();
        for (size_t i = 0; i < procList.size(); i++)
            print_arch(procList[i], prefix, deep+1);
        return;
    }

    if (ia != NULL)
    {
        std::cout << prefix << ia->getProcessorName()
                  << '{' << (IProcessor *)ia->getPrevProcessor().get()
                  << ',' << (IAdapter   *)ia
                  << ',' << (IProcessor *)ia->getNextProcessor().get() << '}';
        print_arch(ia->getNextProcessor(), prefix + "  ", deep+1);
        return;
    }

    if (ip != NULL)
    {
        std::cout << std::endl << prefix << ip->getProcessorName()
                  << '{' << (IProcessor *)ip->getPrevProcessor().get()
                  << ',' << (IProcessor *)ip
                  << ',' << (IProcessor *)ip->getNextProcessor().get() << '}'
                  << "(" << ip->getProtocol().name << ")";
        print_arch(ip->getNextProcessor(), prefix + "  ", deep+1);
        return;
    }
}

void connect(IProcessorPtr a, IProcessorPtr b)
{
    a->getNextProcessor()->setNextProcessor(b->getPointer());
}

int main(int argc, char * argv[])
{
    #define NEW_Connector IProcessorPtr(new ProtocolConnector())

    IAdapterPtr pcap1Adapter(new PcapAdapter(NEW_Connector));
    IAdapterPtr pcap2Adapter(new PcapAdapter(NEW_Connector));

    IProcessorPtr mac1HeaderProcessor(new MacHeaderProcessor(NEW_Connector));
    IProcessorPtr mac2HeaderProcessor(new MacHeaderProcessor(NEW_Connector));

    IProcessorPtr macSwitch(new MacSwitch(NEW_Connector));

    IProcessorPtr acceptProcessor(new AcceptProcessor(NEW_Connector));
    IProcessorPtr ipSplitter(new Ipv4Splitter(NEW_Connector));
    IProcessorPtr ipHeaderProcessor(new Ipv4HeaderProcessor(NEW_Connector));
    
    IProcessorPtr icmpProcessor(new IcmpProcessor(NEW_Connector));
    IProcessorPtr tcpSplitter(new TcpSplitter(NEW_Connector));
    IProcessorPtr tcpOptionsRemover(new TcpOptionsRemover(NEW_Connector));
    IProcessorPtr tcpLayerProcessor(new TcpLayerProcessor(NEW_Connector));
    IProcessorPtr tcpHeaderProcessor(new TcpHeaderProcessor(NEW_Connector));

    IProcessorPtr telnetProcessor(new TelnetSwapper(NEW_Connector));
    IProcessorPtr httpDefragProcessor(new HttpDefragProcessor(NEW_Connector));
    
    /*
    ProcessorModule * macModule  = new ProcessorModule(macHeaderProcessor);
    ProcessorModule * ipModule = new ProcessorModule(ipHeaderProcessor);
    ProcessorModule * tcpModule  = new ProcessorModule(portProcessor);
    ProcessorModule * icmpModule = new ProcessorModule(icmpProcessor);
    */

    pcap1Adapter->setSelf(pcap1Adapter);
    pcap2Adapter->setSelf(pcap2Adapter);
    mac1HeaderProcessor->setSelf(mac1HeaderProcessor);
    mac2HeaderProcessor->setSelf(mac2HeaderProcessor);

    macSwitch->setSelf(macSwitch);

    acceptProcessor->setSelf(acceptProcessor);
    ipSplitter->setSelf(ipSplitter);
    ipHeaderProcessor->setSelf(ipHeaderProcessor);

    icmpProcessor->setSelf(icmpProcessor);
    tcpSplitter->setSelf(tcpSplitter);
    tcpOptionsRemover->setSelf(tcpOptionsRemover);
    tcpLayerProcessor->setSelf(tcpLayerProcessor);
    tcpHeaderProcessor->setSelf(tcpHeaderProcessor);

    telnetProcessor->setSelf(telnetProcessor);
    httpDefragProcessor->setSelf(httpDefragProcessor);

    //////////////////////////////////////////////////////////////////////////

    connect(pcap1Adapter, mac1HeaderProcessor);
    connect(pcap2Adapter, mac2HeaderProcessor);
    
    connect(mac1HeaderProcessor, macSwitch);
    connect(mac2HeaderProcessor, macSwitch);

    connect(macSwitch, ipSplitter);
    connect(macSwitch, acceptProcessor);
        connect(ipSplitter, ipHeaderProcessor);
        connect(ipHeaderProcessor, icmpProcessor);
        connect(ipHeaderProcessor, tcpSplitter);
            connect(tcpSplitter, tcpOptionsRemover);
            connect(tcpOptionsRemover, tcpLayerProcessor);
            connect(tcpLayerProcessor, tcpHeaderProcessor);
            connect(tcpHeaderProcessor, telnetProcessor);//httpDefragProcessor);
  
    pcap1Adapter->ping(IProcessorPtr());
    pcap2Adapter->ping(IProcessorPtr());

    //////////////////////////////////////////////////////////////////////////

    SwitchOption * opt1 = (SwitchOption *)pcap1Adapter->getOptions().get();
    for (unsigned i = 0; i < opt1->getTextItems().size(); i++)
        std::cout << i << ". " << opt1->getTextItems()[i] << std::endl;

    int index1;
    std::cin >> index1;
    opt1->setSelectedIndex(index1);

    SwitchOption * opt2 = (SwitchOption *)pcap2Adapter->getOptions().get();
    for (unsigned i = 0; i < opt2->getTextItems().size(); i++)
        std::cout << i << ". " << opt2->getTextItems()[i] << std::endl;

    int index2;
    std::cin >> index2;
    opt2->setSelectedIndex(index2);

    //////////////////////////////////////////////////////////////////////////

    /*
    print_arch(pcap1Adapter);
    std::cout << std::endl << std::endl;
    print_arch(pcap2Adapter);
    std::cout << std::endl << std::endl;
    */

    //////////////////////////////////////////////////////////////////////////

    pcap1Adapter->run(false);
    pcap2Adapter->run(false);

    if (pcap1Adapter->getProcessorName() == std::string("FileAdapter"))
        fileAdapterPairReader(pcap1Adapter, pcap2Adapter);
    else
        while (true)
        {
            pcap1Adapter->tick();
            pcap2Adapter->tick();
        }
}