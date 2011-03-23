#include "FileAdapter.h"
#include "RawPacket.h"
#include <list>
#include <iostream>

using namespace DiplomBukov;

FileAdapter::FileAdapter(const std::string & filename1,
                         const std::string & filename2,
                         IProcessorPtr Connector)
	: file1(NULL), file2(NULL), id(0), linkType(0), buffer(NULL)
{
    setNextProcessor(Connector);

	if (fopen_s(&file1, filename1.c_str(), "rb") != 0)
    {
        std::cerr << "Error opening file \"" << filename1 << '"' << std::endl;
		throw 1;
    }

    if (!filename2.empty())
    if (fopen_s(&file2, filename2.c_str(), "wb") != 0)
    {
        std::cerr << "Error opening file \"" << filename2 << '"' << std::endl;
        throw 1;
    }
}

IProcessorPtr FileAdapter::CreateCopy() const
{
    return IProcessorPtr();
}

FileAdapter::~FileAdapter()
{
	if (file1 != NULL)
		fclose(file1);
    if (file2 != NULL)
        fclose(file2);
    delete [] buffer;
}

const char * FileAdapter::getProcessorName()
{
    return "FileAdapter";
}


ProcessingStatus FileAdapter::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if (file2 == NULL) return ProcessingStatus::Accepted;

    if (packet->status() == IPacket::Rejected)
        return ProcessingStatus::Accepted;

    pcap_packet_header pph;
    pph.ts_sec = packet->time() >> 32;
    pph.ts_usec = packet->time() & 0xffffffff;
    pph.incl_len = packet->realSize();
    pph.orig_len = packet->realSize();
    fwrite(&pph, sizeof(pph), 1, file2);

    fwrite(&packet->data()[offset], 1, packet->size() - offset, file2);
    
    return ProcessingStatus::Accepted;
}

void FileAdapter::run(bool always)
{
    pcap_file_header pfh;
    fread_s(&pfh, sizeof(pfh), 1, sizeof(pfh), file1);
    if (file2 != NULL)
        fwrite(&pfh, sizeof(pfh), 1, file2);

    id = 1;
    linkType = pfh.network;
    buffer = new u8 [65536];

    while (always)
	{
        if (!tick()) break;
	}
}

bool FileAdapter::tick()
{
    int ret;
    pcap_packet_header pph;
    ret = fread_s(&pph, sizeof(pph), 1, sizeof(pph), file1);
    if (ret == 0) return false;

    ret = fread_s(buffer, 65536, 1, pph.orig_len, file1);
    if (ret == 0) return false;

    IPacketPtr packet(new RawPacket(buffer, ret));
    packet->setId(id++);
    packet->setTime(pph.ts_sec, pph.ts_usec);
    packet->setAdapter(this);
    packet->addProcessor(Self);

    Protocol::PhysicalLayer proto = (Protocol::PhysicalLayer)linkType;
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(proto, packet, 0);

    return true;
}