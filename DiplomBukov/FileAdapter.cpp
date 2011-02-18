#include "FileAdapter.h"
#include <list>

using namespace DiplomBukov;

FileAdapter::FileAdapter(char * filename1, char * filename2, IRouter * router_)
	: router_(router_), file1(NULL), file2(NULL)
{
	int ret = fopen_s(&file1, filename1, "rb");
	if (ret != 0)
		throw "Ошибка открытия файла";

    ret = fopen_s(&file2, filename2, "wb");
    if (ret != 0)
        throw "Ошибка открытия файла";
}

FileAdapter::~FileAdapter()
{
	if (file1 != NULL)
		fclose(file1);
    if (file2 != NULL)
        fclose(file2);
}

void FileAdapter::setRouter(IRouter * router_)
{
	this->router_ = router_;
}

IRouter * FileAdapter::router()
{
	return router_;
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

        Packet packet(pph.incl_len, id++);
        packet.time = pph.ts_sec;

        if (fread_s(packet.data, packet.size, 1, pph.orig_len, file1) == 0)
            break;
        
        router_->transmitPacket(pfh.network, packet, 0); // Protocol::Ethernet_II

        if (packet.status == Packet::Accepted)
        {
            pph.incl_len = packet.real_size;
            pph.orig_len = packet.real_size;
            fwrite(&pph, sizeof(pph), 1, file2);
            fwrite(packet.data, packet.real_size, 1, file2);
        }
	}
}