#include "FileAdapter.h"
#include "RawPacket.h"
#include <list>

using namespace DiplomBukov;

FileAdapter::FileAdapter(char * filename1, char * filename2, IProcessorPtr Connector)
	: file1(NULL), file2(NULL)
{
    setNextProcessor(Connector);

	if (fopen_s(&file1, filename1, "rb") != 0)
		throw "Ошибка открытия файла";

    if (fopen_s(&file2, filename2, "wb") != 0)
        throw "Ошибка открытия файла";
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
}

const char * FileAdapter::getProcessorName()
{
    return "FileAdapter";
}


ProcessingStatus FileAdapter::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if (packet->status() == Packet::Rejected)
        return ProcessingStatus::Accepted;

    pcap_packet_header pph;
    pph.ts_sec = packet->time();
    pph.ts_usec = 0;
    pph.incl_len = packet->realSize();
    pph.orig_len = packet->realSize();
    fwrite(&pph, sizeof(pph), 1, file2);

    char * buf = new char [65536];
    std::copy(packet->data().begin(), packet->data().end(), buf);
    fwrite(buf+offset, packet->realSize()-offset, 1, file2);
    delete [] buf;

    return ProcessingStatus::Accepted;
}

void FileAdapter::run()
{
    pcap_file_header pfh;
    fread_s(&pfh, sizeof(pfh), 1, sizeof(pfh), file1);
    fwrite(&pfh, sizeof(pfh), 1, file2);

    unsigned id = 1;
    std::deque<IPacketPtr> * packets = new std::deque<IPacketPtr>();
    u8 * buf = new u8 [65536];
    while (true)
	{
        int ret;
        pcap_packet_header pph;
        ret = fread_s(&pph, sizeof(pph), 1, sizeof(pph), file1);
        if (ret == 0) break;

        ret = fread_s(buf, 65536, 1, pph.orig_len, file1);
        if (ret == 0)
            break;

        IPacketPtr packet(new RawPacket(buf, ret));
        packet->setId(id++);
        packet->setTime(pph.ts_sec);
        packet->setAdapter(this);
        
        Protocol::PhysicalLayer proto = (Protocol::PhysicalLayer)pfh.network;
        nextProcessor->forwardProcess(proto, packet, 0); // Protocol::Ethernet_II

        packets->push_front(packet);
        
        /*
        if (packet->status() == Packet::Accepted)
        {
            pph.incl_len = packet->realSize();
            pph.orig_len = packet->realSize();
            fwrite(&pph, sizeof(pph), 1, file2);
            fwrite(packet->data(), packet->realSize(), 1, file2);
        }
        */
	}
    delete [] buf;
}