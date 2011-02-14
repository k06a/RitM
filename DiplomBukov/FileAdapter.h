#ifndef FILEADAPTER_H
#define FILEADAPTER_H

#include <stdio.h>

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"

struct pcap_file_header 
{
};

struct pcap_packet_header
{
};

namespace DiplomBukov
{
	class FileAdapter :	public IAdapter
	{
		IRouter * router_;
		FILE * file;

	public:
		FileAdapter(char * filename, IRouter * router_ = NULL);
		~FileAdapter();

		virtual void setRouter(IRouter * router_);
		virtual IRouter * router();
		virtual void run();
	};
	// class FileAdapter
}
// namespace DiplomBukov

#endif // FILEADAPTER_H