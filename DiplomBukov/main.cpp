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
#include "IcmpHeaderProcessor.h"
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

#include "ConsoleOptionWalket.h"

using namespace DiplomBukov;

void print_arch(ProcessorPtr proc, std::string prefix = "", int deep = 0)
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
        const std::deque<ProcessorPtr> & procList = ic->nextProcessors();
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

void connect(ProcessorPtr a, ProcessorPtr b)
{
    a->getNextProcessor()->setNextProcessor(b->getPointer());
}

int main(int argc, char * argv[])
{
    #define NEW_Connector ProcessorPtr(new ProtocolConnector())

    AdapterPtr pcap1Adapter(new PcapAdapter(NEW_Connector));
    AdapterPtr pcap2Adapter(new PcapAdapter(NEW_Connector));

    ProcessorPtr mac1HeaderProcessor(new MacHeaderProcessor(NEW_Connector));
    ProcessorPtr mac2HeaderProcessor(new MacHeaderProcessor(NEW_Connector));

    ProcessorPtr macSwitch(new MacSwitch(NEW_Connector));

    ProcessorPtr acceptProcessor(new AcceptProcessor(NEW_Connector));
    ProcessorPtr ipSplitter(new Ipv4Splitter(NEW_Connector));
    ProcessorPtr ipHeaderProcessor(new Ipv4HeaderProcessor(NEW_Connector));
    
    ProcessorPtr icmpProcessor(new IcmpHeaderProcessor(NEW_Connector));
    ProcessorPtr tcpSplitter(new TcpSplitter(NEW_Connector));
    ProcessorPtr tcpOptionsRemover(new TcpOptionsRemover(NEW_Connector));
    ProcessorPtr tcpLayerProcessor(new TcpLayerProcessor(NEW_Connector));
    ProcessorPtr tcpHeaderProcessor(new TcpHeaderProcessor(NEW_Connector));

    ProcessorPtr telnetProcessor(new TelnetSwapper(NEW_Connector));
    ProcessorPtr httpDefragProcessor(new HttpDefragProcessor(NEW_Connector));
    
    /*
    ProcessorModule * macModule  = new ProcessorModule(macHeaderProcessor);
    ProcessorModule * ipModule = new ProcessorModule(ipHeaderProcessor);
    ProcessorModule * tcpModule  = new ProcessorModule(portProcessor);
    ProcessorModule * icmpModule = new ProcessorModule(icmpProcessor);
    */

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
  
    pcap1Adapter->ping(ProcessorPtr());
    pcap2Adapter->ping(ProcessorPtr());

    //////////////////////////////////////////////////////////////////////////
    
    ConsoleOptionWalkerPtr walker(new ConsoleOptionWalker());
    walker->visit(pcap1Adapter);
    walker->visit(pcap2Adapter);

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

    pcap1Adapter->DestroyHierarchy();
    pcap2Adapter->DestroyHierarchy();
}