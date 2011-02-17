#ifndef FILEADAPTER_H
#define FILEADAPTER_H

#include <stdio.h>

#include "IRouter.h"
#include "IAdapter.h"
#include "IProcessor.h"

namespace DiplomBukov
{
	struct pcap_file_header 
	{
		unsigned   int magic_number;   /* magic number */
		unsigned short version_major;  /* major version number */
		unsigned short version_minor;  /* minor version number */
		  signed   int thiszone;       /* GMT to local correction */
		unsigned   int sigfigs;        /* accuracy of timestamps */
		unsigned   int snaplen;        /* max length of captured packets, in octets */
		unsigned   int network;        /* data link type */
	};
	
	struct pcap_packet_header
	{
		unsigned int ts_sec;	/* timestamp seconds */
		unsigned int ts_usec;   /* timestamp microseconds */
		unsigned int incl_len;  /* number of octets of packet saved in file */
		unsigned int orig_len;  /* actual length of packet */
	};

	class FileAdapter :	public IAdapter
	{
		IRouter * router_;
        FILE * file1;
        FILE * file2;

	public:
		FileAdapter(char * filename1, char * filename2, IRouter * router_ = NULL);
		~FileAdapter();

		virtual void setRouter(IRouter * router_);
		virtual IRouter * router();
		virtual void run();
	};
	// class FileAdapter
}
// namespace DiplomBukov

#endif // FILEADAPTER_H