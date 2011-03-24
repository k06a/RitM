#ifndef FILEADAPTER_H
#define FILEADAPTER_H

#include <stdio.h>
#include <string>

#include "AbstractProcessor.h"
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
        : public AbstractProcessor
        , public IAdapter
	{
		FILE * file1;
        FILE * file2;

        unsigned id;
        int linkType;
        u8 * buffer;

	public:
        FileAdapter(const std::string & filename1,
                    const std::string & filename2 = "",
                    IProcessorPtr Connector = IProcessorPtr());
		virtual IProcessorPtr CreateCopy() const;
        ~FileAdapter();
        
        virtual ProcessingStatus backwardProcess(Protocol proto, IPacketPtr packet, unsigned offset);

        virtual const char * getProcessorName();

		virtual void run(bool always);
        virtual bool tick();
	};
	// class FileAdapter
}
// namespace DiplomBukov

#endif // FILEADAPTER_H