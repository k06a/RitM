#include "DnsMessageProcessor.h"
#include <algorithm>
#include "IPacket.h"
#include "network/ipv4_header.h"
#include "GroupOption.h"

using namespace DiplomBukov;

DnsMessageProcessor::DnsMessageProcessor(ProcessorPtr processor)
    : options(new GroupOption(true))
{
    setNextProcessor(processor);

    check = CheckOptionPtr(new CheckOption(false, "Вкл/Выкл"));
    source = TextLineOptionPtr(new TextLineOption("www.example.com"));
    destType = ComboOptionPtr(new ComboOption);
    destType->addTextItem("IPv4");
    destType->addTextItem("Domain Name");
    destType->addTextItem("Mail Exchange");
    destination = TextLineOptionPtr(new TextLineOption("192.168.1.1"));

    GroupOptionPtr group(new GroupOption(false));
    group->addOption(OptionPtr(check));
    group->addOption(OptionPtr(source));
    group->addOption(OptionPtr(destType));
    group->addOption(OptionPtr(destination));

    alwaysResave = CheckOptionPtr(new CheckOption(false, "Перепаковывать все пакеты"));
    options->addOption(alwaysResave);
    options->addOption(group);
}

ProcessorPtr DnsMessageProcessor::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    return ProcessorPtr(new DnsMessageProcessor(np));
}

ProcessingStatus DnsMessageProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(shared_from_this());

    u16 answerType = 0;
    if (destType->getSelectedText() == "IPv4")
        answerType = DnsRequest::A;
    if (destType->getSelectedText() == "Domain Name")
        answerType = DnsRequest::TXT;
    if (destType->getSelectedText() == "Mail Exchange")
        answerType = DnsRequest::MX;

    dnsMessage.parse(&*(packet->dataBegin() + offset), packet->size()-offset);

    bool podmena = false;

    if (check->isChecked())
    for (unsigned i = 0; i < dnsMessage.answers.size(); i++)
    {
        std::string host = DnsName::readableName(dnsMessage.answers[i].nameSize.first);
        if ((host == source->getText()) &&
            (dnsMessage.answers[i].questionType == answerType))
        {
            switch (answerType)
            {
                case DnsRequest::A:
                {
                    ipv4_addr addr = destination->getText();
                    dnsMessage.answers[i].resources.assign((u8*)&addr, (u8*)&addr+4);
                    break;
                }
                case DnsRequest::TXT:
                {
                    dnsMessage.answers[i].resText =
                        std::make_pair(DnsName::fromString(destination->getText()),0);
                    break;
                }
                case DnsRequest::MX:
                {
                    u16be priority = 0x0028;
                    dnsMessage.answers[i].resources.assign((u8*)&priority, ((u8*)&priority)+2);
                    dnsMessage.answers[i].resText =
                        std::make_pair(DnsName::fromString(destination->getText()),0);
                    break;
                }
            }
            podmena = true;           
        }
    }
    
    if ((podmena) || alwaysResave->isChecked())
    {
        std::vector<u8> vec = dnsMessage.dump();
        packet->resize(offset + vec.size());
        packet->setRealSize(offset + vec.size());
        std::copy(vec.begin(), vec.end(), &(*packet)[offset]);
    }

    backwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus DnsMessageProcessor::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if (packet->processorBefore(shared_from_this()) != NULL)
        packet->processorBefore(shared_from_this())->backwardProcess(proto, packet, offset);
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

OptionPtr DnsMessageProcessor::getOptions()
{
    return options;
}

bool DnsMessageProcessor::isFinal()
{
    return true;
}