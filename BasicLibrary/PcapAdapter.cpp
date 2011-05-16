#include "PcapAdapter.h"
#include "RawPacket.h"
#include "Log.h"
#include "crc.h"
#include <pcap.h>
#include <iostream>
#include <list>

using namespace DiplomBukov;

PcapAdapter::PcapAdapter(ProcessorPtr Connector)
    : statCounter(new BasicStatCounter)
    , devicesSwitch(new SwitchOption)
    , deviceList(NULL)
    , device(NULL)
{
    setNextProcessor(Connector);

    devicesSwitch->setName("Сетевые адаптеры:");
    devicesSwitch->addTextItem("None");

    // Retrieve the device list from the local machine
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs(&deviceList, errbuf) == -1)
    {
        LogLine() << "Ошибка при вызове pcap_findalldevs(): " << errbuf;
        return;
    }
    
    for(pcap_if_t * dev = deviceList; dev != NULL; dev = dev->next)
    {
        std::string about;
        about = about + dev->description + "\n   (" + dev->name + ")";
        devicesSwitch->addTextItem(about.c_str());
    }
}

ProcessorPtr PcapAdapter::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    PcapAdapterPtr p(new PcapAdapter(np));
    p->devicesSwitch->setSelectedIndex(devicesSwitch->getSelectedIndex());

    return p;
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

ProcessingStatus PcapAdapter::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if (packet->status() == IPacket::Rejected)
        return ProcessingStatus::Accepted;

    // Save hash
    u32 hash = crc32(&(*packet)[offset], packet->size() - offset);
    hashes.push_back(hash);

    //std::cout << '-' << devicesSwitch->getSelectedIndex();

    int ret = pcap_sendpacket(device, &(*packet)[offset], packet->size() - offset);
    statCounter->i_count_out++;

    return ProcessingStatus::Accepted;
}


OptionPtr PcapAdapter::getOptions()
{
    return devicesSwitch;
}

void PcapAdapter::run(bool always)
{
    if (devicesSwitch->getSelectedIndex() == 0)
    {
        LogLine() << "Ни один из сетевых адапетеров не выбран. Проверьте настройки.";
    }

    pcap_if_t * dev = deviceList;
    for (int i = 1; i < devicesSwitch->getSelectedIndex(); i++)
        dev = dev->next;
    
    char errbuf[PCAP_ERRBUF_SIZE];

    device = pcap_open_live(
        dev->name,  // name of the device
        65536,      // portion of the packet to capture. 
        1,          // PCAP_OPENFLAG_PROMISCUOUS, for promiscuous mode
        1,          // read timeout
        errbuf);    // error buffer

    if (device == NULL)
    {
        LogLine() << "Не удалось открыть сетевой адаптер: "
                   << dev->name
                   << '(' << dev->description << ')';
        return;
    }

    id = 0;
    linkType = pcap_datalink(device);
    while (always)
    {
        tick();
    }
}

bool PcapAdapter::tick()
{
    pcap_pkthdr header;
    const u8 * pkt_data = pcap_next(device, &header);
    if (pkt_data == NULL)
        return true;

    // Find in hash
    u32 hash = crc32(pkt_data, header.caplen);
    std::deque<u32>::iterator it = std::find(hashes.begin(), hashes.end(), hash);
    if (it != hashes.end())
    {
        hashes.erase(it);
        return true;
    }

    PacketPtr packet(new RawPacket(pkt_data, header.caplen));
    packet->setStatus(IPacket::Accepted);
    packet->setId(id++);
    packet->setTime(header.ts.tv_sec, header.ts.tv_usec);
    packet->setAdapter(this);
    packet->addProcessor(shared_from_this());

    //std::cout << '+' << devicesSwitch->getSelectedIndex();

    Protocol::PhysicalLayer proto = (Protocol::PhysicalLayer)linkType;
    if (nextProcessor != NULL)
        nextProcessor->forwardProcess(proto, packet, 0);
    
    statCounter->i_count_in++;
    return true;
}

IAdapter::Type PcapAdapter::type()
{
    return Online;
}

StatsProviderPtr PcapAdapter::statsProvider()
{
    return statCounter;
}