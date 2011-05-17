#include "DnsMessageProcessor.h"
#include <algorithm>
#include "IPacket.h"
#include "network/ipv4_header.h"
#include "GroupOption.h"

using namespace RitM;

DnsMessageProcessor::AppendLitener::AppendLitener(GroupOptionPtr groupOption)
    : groupOption(groupOption)
{
}

void DnsMessageProcessor::AppendLitener::clicked()
{
    OptionPtr opt = groupOption->options_item(0)->CreateCopy();
    groupOption->addOption(opt);
}

DnsMessageProcessor::RemoveLitener::RemoveLitener(GroupOptionPtr groupOption)
    : groupOption(groupOption)
{
}

void DnsMessageProcessor::RemoveLitener::clicked()
{
    if (groupOption->options_size() > 1)
        groupOption->removeLastOption();
}

DnsMessageProcessor::DnsMessageProcessor(ProcessorPtr processor)
    : options_list(new GroupOption(true))
    , options(new GroupOption(true))
    , appendListener(new AppendLitener(options_list))
    , removeListener(new RemoveLitener(options_list))
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

    options_list->addOption(group);

    alwaysResave = CheckOptionPtr(new CheckOption(false, "Перепаковывать все пакеты"));

    PushButtonOptionPtr addButton(new PushButtonOption("Добавить"));
    addButton->setListener(appendListener);
    PushButtonOptionPtr delButton(new PushButtonOption("Удалить"));
    delButton->setListener(removeListener);
    GroupOptionPtr buttons(new GroupOption(false));
    buttons->addOption(addButton);
    buttons->addOption(delButton);

    options->addOption(alwaysResave);
    options->addOption(options_list);
    options->addOption(buttons);
}

ProcessorPtr DnsMessageProcessor::CreateCopy() const
{
    ProcessorPtr np = ProcessorPtr();
    if (nextProcessor != NULL)
        np = nextProcessor->CreateCopy();

    DnsMessageProcessorPtr p(new DnsMessageProcessor(np));
    p->getOptions()->loadFromString(options->saveToString());

    return p;
}

ProcessingStatus DnsMessageProcessor::forwardProcess(Protocol proto, PacketPtr packet, unsigned offset)
{
    if ((proto != Protocol::None) && (proto != getProtocol()))
        return ProcessingStatus::Rejected;

    packet->addProcessor(shared_from_this());

    /*
    dnsMessage.parse(&*(packet->dataBegin() + offset), packet->size()-offset);

    
    bool podmena = false;
    for (unsigned i = 0; i < dnsMessage.answers.size(); i++)
    {
        for (int line = 0; line < options_list->options_size(); line++)
        {
            GroupOptionPtr gr = SharedPointerCast<GroupOption>(options_list->options_item(line));

            CheckOptionPtr    check       = SharedPointerCast<CheckOption   >(gr->options_item(0));
            TextLineOptionPtr source      = SharedPointerCast<TextLineOption>(gr->options_item(1));
            ComboOptionPtr    destType    = SharedPointerCast<ComboOption   >(gr->options_item(2));
            TextLineOptionPtr destination = SharedPointerCast<TextLineOption>(gr->options_item(3));
            
            if (!check->isChecked())
                continue;

            u16 answerType = 0;
            if (destType->getSelectedText() == "IPv4")
                answerType = DnsRequest::A;
            if (destType->getSelectedText() == "Domain Name")
                answerType = DnsRequest::TXT;
            if (destType->getSelectedText() == "Mail Exchange")
                answerType = DnsRequest::MX;

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

            break;
        }
    }
    
    if ((podmena) || alwaysResave->isChecked())
    {
        std::vector<u8> vec = dnsMessage.dump();
        packet->resize(offset + vec.size());
        packet->setRealSize(offset + vec.size());
        std::copy(vec.begin(), vec.end(), &(*packet)[offset]);
    }

    */
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