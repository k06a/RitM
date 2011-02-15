#include "FileAdapter.h"
#include <list>

using namespace DiplomBukov;

FileAdapter::FileAdapter(char * filename, IRouter * router_)
	: router_(router_), file(NULL)
{
	int ret = fopen_s(&file, filename, "rb");
	if (ret != 0)
		throw "Ошибка открытия файла";
}

FileAdapter::~FileAdapter()
{
	if (file != NULL)
		fclose(file);
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
    fread_s(&pfh, sizeof(pfh), 1, sizeof(pfh), file);

    unsigned id = 0;
    //std::list<Packet*> packets;
	while (true)
	{
        int ret;
        pcap_packet_header pph;
        ret = fread_s(&pph, sizeof(pph), 1, sizeof(pph), file);
        if (ret == 0) break;

        Packet * packet = new Packet();
        //packets.push_back(packet);
        packet->id = id++;
        packet->time = pph.ts_sec;

        ret = fread_s(packet->data, sizeof(packet->data), 1, pph.incl_len, file);
        if (ret == 0) break;
        
        router_->transmitPacket(Protocol::None, *packet, 0);

        delete packet;
	}
}