#include "DnsMessageProcessor.h"
#include <algorithm>
#include "IPacket.h"
#include "network\ipv4_header.h"
#include "GroupOption.h"

using namespace DiplomBukov;

DnsMessageProcessor::DnsMessageProcessor(ProcessorPtr processor)
    : options(new GroupOption(false))
{
    setNextProcessor(processor);

    std::deque<std::string> types;
    types.push_back("IPv4");
    types.push_back("Domain Name");
    types.push_back("Mail Exchange");

    check = new CheckOption(false, "Вкл/Выкл");
    source = new TextLineOption("www.example.com");
    destType = new SwitchOption(types);
    destination = new TextLineOption("192.168.1.1");

    options->addOption(OptionPtr(check));
    options->addOption(OptionPtr(source));
    options->addOption(OptionPtr(destType));
    options->addOption(OptionPtr(destination));
}

ProcessorPtr DnsMessageProcessor::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        nextProcessor->CreateCopy();

    ProcessorPtr ptr(new DnsMessageProcessor(np));
    ptr->setSelf(ptr);
    return ptr;
}

ProcessingStatus DnsMessageProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(Self);

    u16 answerType = 0;
    if (destType->getSelectedText() == "IPv4")
        answerType = DnsRequest::A;
    if (destType->getSelectedText() == "Domain Name")
        answerType = DnsRequest::TXT;
    if (destType->getSelectedText() == "Mail Exchange")
        answerType = DnsRequest::MX;

    dnsMessage.parse(&packet->data()[offset], packet->size()-offset);

    bool podmena = false;

    // Подмена ответа
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
                    //dnsMessage.answers[i].resText = 
                    dnsMessage.answers[i].resText =
                        std::make_pair(DnsName::fromString(destination->getText()),0);
                    break;
                }
                case DnsRequest::MX:
                {
                    ipv4_addr addr = destination->getText();
                    dnsMessage.answers[i].resources.assign((u8*)&addr, (u8*)&addr+4);
                    break;
                }
            }
            podmena = true;           
        }
    }
    
    if (podmena)
    {
        std::vector<u8> vec = dnsMessage.dump();
        packet->data().resize(offset + vec.size());
        packet->setRealSize(offset + vec.size());
        std::copy(vec.begin(), vec.end(), &packet->data()[offset]);
    }

    backwardProcess(proto, packet, offset);

    return ProcessingStatus::Accepted;
}

ProcessingStatus DnsMessageProcessor::backwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
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

OptionPtr DnsMessageProcessor::getOptions()
{
    return options;
}