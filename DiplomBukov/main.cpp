#include <deque>
#include <string>
#include <iostream>

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"

#include "FileAdapter.h"
#include "BruteRouter.h"
#include "ProtocolRouter.h"
#include "MacProcessor.h"
#include "Ipv4SplitterProcessor.h"
#include "Ipv4DefragProcessor.h"
#include "TcpSplitterProcessor.h"
#include "UdpSplitterProcessor.h"

using namespace DiplomBukov;

void print_arch(IProcessor * proc, std::string prefix = "")
{
    if (proc == NULL) return;

    std::cout << prefix << proc->getProcessorName() << std::endl;

    std::deque<IProcessor*> procList = proc->router()->nextProcessors();
    for (size_t i = 0; i < procList.size(); i++)
    {
        print_arch(procList[i], prefix + "    ");
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
    //IProcessor * icmpProcessor  = new IcmpProcessor(new ProtocolRouter());

    #define connect(a,b) a->router()->addNextProcessor(b)

    connect(fileAdapter, macProcessor);
        connect(macProcessor, ipv4Splitter);
            connect(ipv4Splitter, ipdfProcessor);
                connect(ipdfProcessor, tcpProcessor);
                connect(ipdfProcessor, udpProcessor);
                //connect(ipdfProcessor, icmpProcessor);

    print_arch(macProcessor);

    fileAdapter->start();
}