#include "PushButtonOption.h"

using namespace RitM;

PushButtonOption::PushButtonOption(const char * name)
    : label(name)
{
}

OptionPtr PushButtonOption::CreateCopy() const
{
    PushButtonOptionPtr ptr(new PushButtonOption(label.c_str()));
    ptr->setName(getName());
    return ptr;
}

const char * PushButtonOption::getName() const
{
    return label.c_str();
}

void PushButtonOption::setName(const char * name)
{
    label = name;
}

void PushButtonOption::visitMe(OptionWalkerPtr walker)
{
    walker->visit(shared_from_this());
}

PushButtonOptionListenerPtr PushButtonOption::getListener()
{
    return listener;
}
void PushButtonOption::setListener(PushButtonOptionListenerPtr someone)
{
    listener = someone;
}

void PushButtonOption::click()
{
    listener->clicked();
}

std::string PushButtonOption::saveToString(int level)
{
    return "";
}

void PushButtonOption::loadFromString(std::string str, int level)
{
}