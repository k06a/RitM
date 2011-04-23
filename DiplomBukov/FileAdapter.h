#ifndef FILEADAPTER_H
#define FILEADAPTER_H

#include <stdio.h>
#include <string>
#include <vector>

#include "AbstractProcessor.h"
#include "IAdapter.h"
#include "IStatsProvider.h"
#include "GroupOption.h"
#include "FileOpenOption.h"

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
        class StatCounter : public IStatsProvider
        {
        public:
            i64 i_count_in;
            i64 i_count_out;

            StatCounter();
            virtual std::vector<std::string> getStatisticNames() const;
            virtual std::vector<i64> getStatisticValues() const;
        };

        typedef SharedPointer<StatCounter>::Type StatCounterPtr;

        StatCounterPtr statCounter;

		FILE * file1;
        FILE * file2;

        unsigned id;
        int linkType;
        u8 * buffer;

        GroupOptionPtr groupOption;
        FileOpenOptionPtr inFile;
        FileOpenOptionPtr outFile;

	public:
        FileAdapter(ProcessorPtr Connector = ProcessorPtr());
		virtual ProcessorPtr CreateCopy() const;
        ~FileAdapter();
        
        virtual ProcessingStatus backwardProcess(Protocol proto, PacketPtr packet, unsigned offset);

        virtual const char * getProcessorName();

		virtual void run(bool always);
        virtual bool tick();

        virtual Type type();

        virtual OptionPtr getOptions();

        virtual StatsProviderPtr statsProvider();
	};
	// class FileAdapter

    typedef SharedPointer<FileAdapter>::Type FileAdapterPtr;
}
// namespace DiplomBukov

#endif // FILEADAPTER_H