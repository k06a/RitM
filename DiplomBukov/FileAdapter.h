#ifndef FILEADAPTER_H
#define FILEADAPTER_H

#include <stdio.h>

#include "AbstractPacketProcessor.h"
#include "IAdapter.h"

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

	class FileAdapter
        : public AbstractPacketProcessor
        , public IAdapter
	{
		FILE * file1;
        FILE * file2;

	public:
		FileAdapter(char * filename1, char * filename2, IPacketProcessor * router = NULL);
		virtual IPacketProcessor * CreateCopy() const;
        ~FileAdapter();
        
		virtual void run();
	};
	// class FileAdapter
}
// namespace DiplomBukov

#endif // FILEADAPTER_H