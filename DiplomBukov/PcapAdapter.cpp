#include "PcapAdapter.h"
#include "RawPacket.h"
#include "crc.h"
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
        about = about + dev->description + "\n   (" + dev->name + ")";
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

//static bool znak;

ProcessingStatus PcapAdapter::backwardProcess(Protocol proto, IPacketPtr & packet, unsigned offset)
{
    if (packet->status() == Packet::Rejected)
        return ProcessingStatus::Accepted;

    // Save hash
    u32 hash = Crc32(&packet->data()[offset], packet->realSize() - offset);
    hashes.push_back(hash);

    /*
    SwitchOption * opt = (SwitchOption *)devicesSwitch.get();
    std::cout << '-' << opt->getSelectedIndex();
    if (!znak)
    {
        znak = false;
    }
    znak = false;
    */

    pcap_sendpacket(device, &packet->data()[offset], packet->realSize() - offset);

    return ProcessingStatus::Accepted;
}

std::deque<IOptionPtr> PcapAdapter::getOptions()
{
    return options;
}

void PcapAdapter::run(bool always)
{
    SwitchOption * opt = (SwitchOption *)devicesSwitch.get();
    pcap_if_t * dev = deviceList;
    for (int i = 0; i < opt->getSelectedIndex(); i++)
        dev = dev->next;
    
    char errbuf[PCAP_ERRBUF_SIZE];

    device = pcap_open_live(
        dev->name,  // name of the device
        65536,      // portion of the packet to capture. 
        1,          // PCAP_OPENFLAG_PROMISCUOUS, for promiscuous mode
        1,          // read timeout
        errbuf);    // error buffer

    if (device == NULL)
        throw std::string("Unable to open the adapter. %s is not supported by WinPcap");

    id = 0;
    linkType = pcap_datalink(device);
    while (always)
    {
        tick();
    }
}

void PcapAdapter::tick()
{
    pcap_pkthdr header;
    const u8 * pkt_data = pcap_next(device, &header);
    if (pkt_data == NULL)
        return;

    // Find in hash
    u32 hash = Crc32(pkt_data, header.caplen);
    std::deque<u32>::iterator it = std::find(hashes.begin(), hashes.end(), hash);
    if (it != hashes.end())
    {
        hashes.erase(it);
        return;
    }

    IPacketPtr packet(new RawPacket(pkt_data, header.caplen));
    packet->setStatus(IPacket::Accepted);
    packet->setId(id++);
    packet->setTime(header.ts.tv_sec);
    packet->setAdapter(this);
    packet->addProcessor(Self);

    /*
    SwitchOption * opt = (SwitchOption *)devicesSwitch.get();
    std::cout << '+' << opt->getSelectedIndex();
    znak = true;
    */

    Protocol::PhysicalLayer proto = (Protocol::PhysicalLayer)linkType;
    nextProcessor->forwardProcess(proto, packet, 0);
}