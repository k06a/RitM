#include "DnsMessageProcessor.h"
#include <algorithm>
#include "IPacket.h"
#include "network\ipv4_header.h"

using namespace DiplomBukov;

DnsMessageProcessor::DnsMessageProcessor(IProcessorPtr processor)
{
    setNextProcessor(processor);
}

IProcessorPtr DnsMessageProcessor::CreateCopy() const
{
    IProcessorPtr np = IProcessorPtr();
    if (nextProcessor != NULL)
        nextProcessor->CreateCopy();

    IProcessorPtr ptr(new DnsMessageProcessor(np));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus DnsMessageProcessor::forwardProcess(Protocol proto, IPacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(Self);

    dnsMessage.clear();
    dnsMessage.Init(&packet->data()[offset], packet->size()-offset);

    // Подмена ответа
    for (unsigned i = 0; i < dnsMessage.answers.size(); i++)
    {
        if (dnsMessage.answers[i].questionType != dns_header::A)
            continue;

        std::string host = DnsSymbolicName::readableName(dnsMessage.answers[i].symbolicName);
        if (host == "www.netbsd.org")
        {
            ipv4_addr addr = "192.168.1.1";
            dnsMessage.answers[i].resources.assign((u8*)&addr, (u8*)&addr+4);
            dnsMessage.header.NSCOUNT = 0;
            dnsMessage.header.ARCOUNT = 0;
            //dnsMessage.header.flags.RA = 0;
            //dnsMessage.header.flags.RD = 0;

            std::vector<u8> vec = dnsMessage.dumpToData();
            packet->data().resize(offset + vec.size());
            packet->setRealSize(offset + vec.size());
            std::copy(vec.begin(), vec.end(), &packet->data()[offset]);
        }
    }

    backwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus DnsMessageProcessor::backwardProcess(Protocol proto, IPacketPtr packet, unsigned offset)
{
    if (packet->processorBefore(Self) != NULL)
        packet->processorBefore(Self)->backwardProcess(proto, packet, offset);
    return ProcessingStatus::Accepted;
}

Protocol DnsMessageProcessor::getProtocol()
{
    return Protocol("UDP_53", 53);
}

const char * DnsMessageProcessor::getProcessorName()
{
    return "DnsMessageProcessor";
}