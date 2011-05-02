#include "SwitchOption.h"
#include <algorithm>

using namespace DiplomBukov;

SwitchOption::SwitchOption()
    : label(), labels(), selected(0)
{
}

SwitchOption::SwitchOption(const char * name)
    : label(name), labels(), selected(0)
{
}

OptionPtr SwitchOption::CreateCopy() const
{
    SwitchOptionPtr ptr(new SwitchOption(getName()));
    for (int i = 0; i < getTextItems_size(); i++)
        ptr->addTextItem(getTextItems_item(i));
    return ptr;
}

const char * SwitchOption::getName() const
{
    return label.c_str();
}

void SwitchOption::setName(const char * text)
{
    label = text;
}

void SwitchOption::visitMe(OptionWalkerPtr walker)
{
    walker->visit(shared_from_this());
}

int SwitchOption::getTextItems_size() const
{
    return labels.size();
}

const char * SwitchOption::getTextItems_item(int i) const
{
    return labels[i].c_str();
}

void SwitchOption::addTextItem(const char * item)
{
    labels.push_back(item);
}

void SwitchOption::removeTextItem(const char * item)
{
    std::string str(item);
    std::remove(labels.begin(), labels.end(), str);
}

int SwitchOption::getIndexOf(const char * item)
{
    std::string str(item);
    std::deque<std::string>::iterator it =
        std::find(labels.begin(), labels.end(), str);
    if (it == labels.end())
        return -1;
    return it - labels.begin();
}

int SwitchOption::getSelectedIndex() const
{
    return selected;
}

void SwitchOption::setSelectedIndex(int index)
{   
    selected = index;
}

const char * SwitchOption::getSelectedText() const
{
    return labels[selected].c_str();
}

std::string SwitchOption::saveToString(int level)
{
    char buf[20];
    sprintf_s(buf, 20, "%d", selected);
    return buf;
}

void SwitchOption::loadFromString(std::string str, int level)
{
    sscanf_s(str.c_str(), "%d", &selected);
    if (selected > getTextItems_size())
        selected = 0;
}