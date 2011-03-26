#include "DnsMessageProcessor.h"
#include <algorithm>
#include "IPacket.h"
#include "network\ipv4_header.h"
#include "GroupOption.h"

using namespace DiplomBukov;

DnsMessageProcessor::DnsMessageProcessor(IProcessorPtr processor)
{
    setNextProcessor(processor);

    std::deque<std::string> types;
    types.push_back("IPv4");
    types.push_back("Text String");
    types.push_back("Mail Exchange");

    GroupOption * group = new GroupOption();
    source = new TextLineOption("www.example.com");
    destType = new SwitchOption(types);
    destination = new TextLineOption("192.168.1.1");

    options = IOptionPtr(group);
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

    dnsMessage.parse(&packet->data()[offset], packet->size()-offset);

    bool podmena = false;
    /*
    // Подмена ответа
    for (unsigned i = 0; i < dnsMessage.answers.size(); i++)
    {
        if (dnsMessage.answers[i].questionType == DnsRequest::A)
        {
            std::string host = DnsName::readableName(dnsMessage.answers[i].nameSize.first);
            if (host == "www.netbsd.org")
            {
                ipv4_addr addr = "17.251.200.70"; // Apple.com IP address :-)
                dnsMessage.answers[i].resources.assign((u8*)&addr, (u8*)&addr+4);

                std::vector<u8> vec = dnsMessage.dump();
                packet->data().resize(offset + vec.size());
                packet->setRealSize(offset + vec.size());
                std::copy(vec.begin(), vec.end(), &packet->data()[offset]);
            }
        }

        if ((dnsMessage.answers[i].questionType == DnsRequest::A) ||
            (dnsMessage.answers[i].questionType == DnsRequest::AAAA))
        {
            std::string host = DnsName::readableName(dnsMessage.answers[i].nameSize.first);
            if (host == "www.isc.org")
            {
                ipv4_addr addr = "77.88.21.11"; // Yandex.ru IP address :-)
                dnsMessage.answers[i].questionType = DnsRequest::A;
                dnsMessage.answers[i].resourceLength = sizeof(ipv4_addr);
                dnsMessage.answers[i].resources.assign((u8*)&addr, (u8*)&addr+4);

                std::vector<u8> vec = dnsMessage.dump();
                packet->data().resize(offset + vec.size());
                packet->setRealSize(offset + vec.size());
                std::copy(vec.begin(), vec.end(), &packet->data()[offset]);
            }
        }
    }
    
    if (podmena)*/
    {
        std::vector<u8> vec = dnsMessage.dump();
        packet->data().resize(offset + vec.size());
        packet->setRealSize(offset + vec.size());
        std::copy(vec.begin(), vec.end(), &packet->data()[offset]);
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

IOptionPtr DnsMessageProcessor::getOptions()
{
    return options;
}