#include "SwitchOption.h"
#include <algorithm>

using namespace DiplomBukov;

SwitchOption::SwitchOption()
    : label(), labels(), selected(0)
{
}

SwitchOption::SwitchOption(const std::string & item,
                           const std::string & name)
    : label(name), labels(1, item), selected(0)
{
}

SwitchOption::SwitchOption(const std::deque<std::string> & items,
                           const std::string & name)
    : label(name), labels(items), selected(0)
{
}

OptionPtr SwitchOption::CreateCopy() const
{
    SwitchOption * ptr = new SwitchOption(labels);
    ptr->setName(getName());
    return OptionPtr(ptr);
}

const std::string & SwitchOption::getName() const
{
    return label;
}

void SwitchOption::setName(const std::string & text)
{
    label = text;
}

const std::deque<std::string> & SwitchOption::getTextItems() const
{
    return labels;
}

void SwitchOption::setTextItems(const std::deque<std::string> & items)
{
    labels = items;
}

void SwitchOption::addTextItem(const std::string & item)
{
    labels.push_back(item);
}

void SwitchOption::removeTextItem(const std::string & item)
{
    std::remove(labels.begin(), labels.end(), item);
}

int SwitchOption::getIndexOf(const std::string & item)
{
    std::deque<std::string>::iterator it =
        std::find(labels.begin(), labels.end(), item);
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

std::string SwitchOption::getSelectedText() const
{
    return labels[selected];
}