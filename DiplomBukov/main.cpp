#include <iostream>

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"

#include "FileAdapter.h"
#include "BruteRouter.h"
#include "ProtocolRouter.h"
#include "MacProcessor.h"
#include "IpProcessor.h"
#include "TcpProcessor.h"

using namespace DiplomBukov;

int main(int argc, char * argv[])
{
    IAdapter * fileAdapter    = new FileAdapter("2.pcap", new ProtocolRouter());
	IProcessor * macProcessor = new MacProcessor(new ProtocolRouter());
    IProcessor * ipProcessor  = new IpProcessor(new ProtocolRouter());
    IProcessor * tcpProcessor = new TcpProcessor(new ProtocolRouter());

    fileAdapter->router()->addNextProcessor(macProcessor);
	macProcessor->router()->addNextProcessor(ipProcessor);
    ipProcessor->router()->addNextProcessor(tcpProcessor);

    fileAdapter->start();
}