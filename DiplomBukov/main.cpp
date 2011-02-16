#include <iostream>

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"

#include "FileAdapter.h"
#include "BruteRouter.h"
#include "ProtocolRouter.h"
#include "MacProcessor.h"
#include "Ipv4Processor.h"
#include "IpDefragProcessor.h"
#include "TcpProcessor.h"

using namespace DiplomBukov;

int main(int argc, char * argv[])
{
    IAdapter   * fileAdapter    = new FileAdapter("2.pcap", new ProtocolRouter());
	IProcessor * macProcessor   = new MacProcessor(new ProtocolRouter());
    IProcessor * ipv4Processor  = new Ipv4Processor(new ProtocolRouter());
    IProcessor * ipdfProcessor  = new IpDefragProcessor(new ProtocolRouter());
    IProcessor * tcpProcessor   = new TcpProcessor(new ProtocolRouter());
    //IProcessor * udpProcessor   = new UdpProcessor(new ProtocolRouter());
    //IProcessor * icmpProcessor  = new IcmpProcessor(new ProtocolRouter());

    #define connect(a,b) a->router()->addNextProcessor(b)

    connect(fileAdapter, macProcessor);
        connect(macProcessor, ipv4Processor);
            connect(ipv4Processor, ipdfProcessor);
                connect(ipdfProcessor, tcpProcessor);
                //connect(ipdfProcessor, udpProcessor);
                //connect(ipdfProcessor, icmpProcessor);

    fileAdapter->start();
}