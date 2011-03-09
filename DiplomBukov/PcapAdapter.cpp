#include "PcapAdapter.h"
#include "RawPacket.h"
#include <list>

using namespace DiplomBukov;

PcapAdapter::PcapAdapter(std::string adapterName, IProcessorPtr Connector)
{
    int i=0;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Retrieve the device list from the local machine
    if (pcap_findalldevs(&deviceList, errbuf) == -1)
        throw std::string("Error in pcap_findalldevs_ex: %s\n") + errbuf;
    
    // Print the list
    for(device = deviceList; device != NULL; device = device->next)
    {
        std::string name = device->name;
        std::string descr = device->description;
        std::string::iterator it =
            std::search(name.begin(), name.end(),
                        adapterName.begin(),
                        adapterName.end());

        if (it == name.end())
        {
            it = std::search(name.begin(), name.end(),
                             adapterName.begin(),
                             adapterName.end());
        }

        if (it != name.end())
        {
            return;
        }
    }

    throw "No such device found";
}

IProcessorPtr PcapAdapter::CreateCopy() const
{
    return IProcessorPtr();
}

PcapAdapter::~PcapAdapter()
{
	pcap_freealldevs(deviceList);
}

const char * PcapAdapter::getProcessorName()
{
    return "PcapAdapter";
}


ProcessingStatus PcapAdapter::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if (packet->status() == Packet::Rejected)
        return ProcessingStatus::Accepted;

    /*
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
    */

    return ProcessingStatus::Accepted;
}

void PcapAdapter::run()
{
    /*
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
        
        //if (packet->status() == Packet::Accepted)
        //{
        //    pph.incl_len = packet->realSize();
        //    pph.orig_len = packet->realSize();
        //    fwrite(&pph, sizeof(pph), 1, file2);
        //    fwrite(packet->data(), packet->realSize(), 1, file2);
        //}
	}
    delete [] buf;
    */
}