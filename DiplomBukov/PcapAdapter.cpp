#include "PcapAdapter.h"
#include "RawPacket.h"
#include <pcap.h>
#include <list>

using namespace DiplomBukov;

PcapAdapter::PcapAdapter(IProcessorPtr Connector)
    : devicesSwitch(IOptionPtr())
    , options()
    , deviceList(NULL)
    , device(NULL)
{
    setNextProcessor(Connector);

    char errbuf[PCAP_ERRBUF_SIZE];

    // Retrieve the device list from the local machine
    if (pcap_findalldevs(&deviceList, errbuf) == -1)
        throw std::string("Error in pcap_findalldevs: ") + errbuf + "\n";
    
    std::deque<std::string> arr;
    for(pcap_if_t * dev = deviceList; dev != NULL; dev = dev->next)
    {
        std::string about;
        about = about + dev->description + " (" + dev->name + ")";
        arr.push_back(about);
    }
    
    SwitchOption * opt = new SwitchOption();
    opt->setTextItems(arr);
    devicesSwitch = IOptionPtr(opt);

    options.push_back(devicesSwitch);
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

std::deque<IOptionPtr> PcapAdapter::getOptions()
{
    return options;
}

void PcapAdapter::run()
{
    SwitchOption * opt = (SwitchOption *)devicesSwitch.get();
    pcap_if_t * dev = deviceList;
    for (int i = 0; i < opt->getSelectedIndex(); i++)
        dev = dev->next;
    
    char errbuf[PCAP_ERRBUF_SIZE];

    /* Open the device */
    pcap_t * adhandle = pcap_open(
        dev->name,          // name of the device
        65536,            // portion of the packet to capture. 
        // 65536 guarantees that the whole packet will be captured on all the link layers
        1, //PCAP_OPENFLAG_PROMISCUOUS,    // promiscuous mode
        1000,             // read timeout
        NULL,             // authentication on the remote machine
        errbuf);            // error buffer

    if (adhandle == NULL)
    {
        throw std::string("Unable to open the adapter. %s is not supported by WinPcap");
        return;
    }

    int id = 0;
    while (true)
    {
        pcap_pkthdr header;
        u8 * pkt_data = pcap_next(adhandle, &header);
        
        IPacketPtr packet(new RawPacket(pkt_data, header->caplen));
        packet->setId(id++);
        packet->setTime(header->ts.tv_sec);
        packet->setAdapter(this);

        Protocol::PhysicalLayer proto = Protocol::PhysicalLayer::Ethernet_II;
        nextProcessor->forwardProcess(proto, packet, 0); // Protocol::Ethernet_II
    }

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