#include <iostream>
#include "IRouter.h"
#include "IProcessor.h"
#include "BruteRouter.h"
#include "MacProcessor.h"
#include "IpProcessor.h"

using namespace DiplomBukov;

int main(int argc, char * argv[])
{
	IProcessor * macProcessor = new MacProcessor(new BruteRouter());
	IProcessor * ipProcessor = new IpProcessor(new BruteRouter());

	macProcessor->router()->addNextProcessor(ipProcessor);
}