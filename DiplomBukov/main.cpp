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
#include "TransportPortProcessor.h"
#include "HttpSwapProcessor.h"

#include "Ipv4Splitter.h"
#include "Ipv4Defragger.h"
#include "TcpSplitter.h"
#include "UdpSplitter.h"

using namespace DiplomBukov;

void print_arch(IProcessorPtr proc, std::string prefix = "")
{
    if (proc == NULL) return;

    IProcessor * ipp = proc.get();
    IConnector * ir = dynamic_cast<IConnector*>(ipp);
    IAdapter   * ia = dynamic_cast<IAdapter*>(ipp);
    IProcessor * ip = dynamic_cast<IProcessor*>(ipp);

    if (ir != NULL)
    {
        std::cout << " [ Connector ]:";
        const std::deque<IProcessorPtr> & procList = ir->nextProcessors();
        for (size_t i = 0; i < procList.size(); i++)
            print_arch(procList[i], prefix + "    ");
        return;
    }

    if (ia != NULL)
    {
        std::cout << prefix << ia->getProcessorName();
        print_arch(ipp->getNextProcessor(), prefix + "    ");
        return;
    }

    if (ip != 0)
    {
        std::cout << std::endl << prefix << ip->getProcessorName()
                  << "(" << ip->getProtocol().name << ")";
        print_arch(ipp->getNextProcessor(), prefix + "    ");
        return;
    }
}

int main(int argc, char * argv[])
{
    #define NEW_Connector IProcessorPtr(new ProtocolConnector())

    IAdapterPtr fileAdapter(new FileAdapter("tcp.pcap", "tcp_out.pcap", NEW_Connector));
    IAdapterPtr pcapAdapter(new PcapAdapter(NEW_Connector));

    /*
	IProcessorPtr macProcessor(new MacHeaderProcessor(NEW_Connector));
    IProcessorPtr ipv4Splitter(new Ipv4Splitter(NEW_Connector));
    IProcessorPtr ipdfProcessor(new Ipv4Defragger(NEW_Connector));
    IProcessorPtr tcpProcessor(new TcpSplitter(NEW_Connector));
    IProcessorPtr udpProcessor(new UdpSplitter(NEW_Connector));
    IProcessorPtr icmpProcessor(new IcmpProcessor(NEW_Connector));

    ProcessorModule * macModule  = new ProcessorModule(macProcessor);
    ProcessorModule * ipv4Module = new ProcessorModule(ipv4Splitter);
    ProcessorModule * ipdfModule = new ProcessorModule(ipdfProcessor);
    ProcessorModule * tcpModule  = new ProcessorModule(tcpProcessor);
    ProcessorModule * udpModule  = new ProcessorModule(udpProcessor);
    ProcessorModule * icmpModule = new ProcessorModule(icmpProcessor);

    #define connect(a,b) a->getNextProcessor()->setNextProcessor(b)

    connect(fileAdapter, macProcessor);
    connect(macProcessor, ipv4Splitter);
            connect(ipv4Splitter, ipdfProcessor);
                connect(ipdfProcessor, tcpProcessor);
                connect(ipdfProcessor, udpProcessor);
                connect(ipdfProcessor, icmpProcessor);
    */
    
    IProcessorPtr macProcessor(new MacHeaderProcessor(NEW_Connector));
    IProcessorPtr ipSplitter(new Ipv4Splitter(NEW_Connector));
    IProcessorPtr ipProcessor(new Ipv4HeaderProcessor(NEW_Connector));
    IProcessorPtr tcpSplitter(new TcpSplitter(NEW_Connector));
    IProcessorPtr portProcessor(new TransportPortProcessor(NEW_Connector));
    IProcessorPtr icmpProcessor(new IcmpProcessor(NEW_Connector));
    IProcessorPtr httpProcessor(new HttpSwapProcessor(NEW_Connector));
    
    ProcessorModule * macModule  = new ProcessorModule(macProcessor);
    ProcessorModule * ipModule = new ProcessorModule(ipProcessor);
    ProcessorModule * tcpModule  = new ProcessorModule(portProcessor);
    ProcessorModule * icmpModule = new ProcessorModule(icmpProcessor);

    #define connect(a,b) a->getNextProcessor()->setNextProcessor(b)

    connect(pcapAdapter, macProcessor);
        connect(macProcessor, ipSplitter);
            connect(ipSplitter, ipProcessor);
                connect(ipProcessor, tcpSplitter);
                    connect(tcpSplitter, portProcessor);
                    connect(portProcessor, httpProcessor);
    
    fileAdapter->setSelf(fileAdapter);
    pcapAdapter->setSelf(pcapAdapter);
    macProcessor->setSelf(macProcessor);
    ipProcessor->setSelf(ipProcessor);
    portProcessor->setSelf(portProcessor);
    icmpProcessor->setSelf(icmpProcessor);
    
    SwitchOption * opt = (SwitchOption *)pcapAdapter->getOptions().front().get();
    for (int i = 0; i < opt->getTextItems().size(); i++)
        std::cout << i << ". " << opt->getTextItems()[i] << std::endl;

    opt->setSelectedIndex(1);

    print_arch(fileAdapter);
    std::cout << std::endl;

    fileAdapter->run();
}