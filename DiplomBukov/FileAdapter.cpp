#include "FileAdapter.h"
#include "RawPacket.h"
#include <list>

using namespace DiplomBukov;

FileAdapter::FileAdapter(char * filename1, char * filename2, IProcessorPtr router)
	: file1(NULL), file2(NULL)
{
    setNextProcessor(router);

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

void FileAdapter::run()
{
    pcap_file_header pfh;
    fread_s(&pfh, sizeof(pfh), 1, sizeof(pfh), file1);
    fwrite(&pfh, sizeof(pfh), 1, file2);

    unsigned id = 1;
    while (true)
	{
        int ret;
        pcap_packet_header pph;
        ret = fread_s(&pph, sizeof(pph), 1, sizeof(pph), file1);
        if (ret == 0) break;

        IPacketPtr packet(new RawPacket(pph.incl_len));
        packet->setId(id++);
        packet->setTime(pph.ts_sec);

        if (fread_s(packet->data(), packet->size(), 1, pph.orig_len, file1) == 0)
            break;
        
        nextProcessor->forwardProcess(pfh.network, packet, 0); // Protocol::Ethernet_II

        if (packet->status() == Packet::Accepted)
        {
            pph.incl_len = packet->realSize();
            pph.orig_len = packet->realSize();
            fwrite(&pph, sizeof(pph), 1, file2);
            fwrite(packet->data(), packet->realSize(), 1, file2);
        }
	}
}