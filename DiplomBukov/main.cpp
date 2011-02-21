#include <deque>
#include <string>
#include <iostream>

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"

#include "ProcessorModule.h"

#include "FileAdapter.h"
#include "BruteRouter.h"
#include "ProtocolRouter.h"
#include "MacProcessor.h"
#include "Ipv4SplitterProcessor.h"
#include "Ipv4DefragProcessor.h"
#include "TcpSplitterProcessor.h"
#include "UdpSplitterProcessor.h"
#include "IcmpProcessor.h"

using namespace DiplomBukov;

void print_arch(IPacketProcessor * proc, std::string prefix = "")
{
    if (proc == NULL) return;

    IPacketProcessor * ipp = proc;
    IRouter * ir = dynamic_cast<IRouter*>(ipp);
    IAdapter * ia = dynamic_cast<IAdapter*>(ipp);
    IProcessor * ip = dynamic_cast<IProcessor*>(ipp);

    if (ir != NULL)
    {
        std::cout << prefix << "[ Router ]:" << std::endl;
        const std::deque<IProcessor*> & procList = ir->nextProcessors();
        for (size_t i = 0; i < procList.size(); i++)
            print_arch(procList[i], prefix + "    ");
        return;
    }

    if (ia != NULL)
    {
        std::cout << prefix << "Adapter" << std::endl;
        print_arch(ipp->getNextProcessor(), prefix + "    ");
        return;
    }

    if (ip != 0)
    {
        std::cout << prefix << ip->getProcessorName() << std::endl;
        print_arch(ipp->getNextProcessor(), prefix + "    ");
        return;
    }
}

int main(int argc, char * argv[])
{
    IAdapter   * fileAdapter   = new FileAdapter("icmp_native_64k.pcap", "icmp_defraged_64k.pcap", new ProtocolRouter());

	IProcessor * macProcessor  = new MacProcessor(new ProtocolRouter());
    IProcessor * ipv4Splitter  = new Ipv4SplitterProcessor(new ProtocolRouter());
    IProcessor * ipdfProcessor = new Ipv4DefragProcessor(new ProtocolRouter());
    IProcessor * tcpProcessor  = new TcpSplitterProcessor(new ProtocolRouter());
    IProcessor * udpProcessor  = new UdpSplitterProcessor(new ProtocolRouter());
    IProcessor * icmpProcessor = new IcmpProcessor(new ProtocolRouter());

    ProcessorModule * macModule  = new ProcessorModule(macProcessor);
    ProcessorModule * ipv4Module = new ProcessorModule(ipv4Splitter);
    ProcessorModule * ipdfModule = new ProcessorModule(ipdfProcessor);
    ProcessorModule * tcpModule  = new ProcessorModule(tcpProcessor);
    ProcessorModule * udpModule  = new ProcessorModule(udpProcessor);
    ProcessorModule * icmpModule = new ProcessorModule(icmpProcessor);

    #define connect(a,b) a->getNextProcessor()->setNextProcessor(b)

    fileAdapter->getNextProcessor()->setNextProcessor(macProcessor);
    connect(fileAdapter, macProcessor);
        connect(macProcessor, ipv4Splitter);
            connect(ipv4Splitter, ipdfProcessor);
                connect(ipdfProcessor, tcpProcessor);
                connect(ipdfProcessor, udpProcessor);
                connect(ipdfProcessor, icmpProcessor);

    print_arch(fileAdapter);

    fileAdapter->run();
}