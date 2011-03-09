#include "SwitchOption.h"

using namespace DiplomBukov;

SwitchOption::SwitchOption()
    : labels(), selected(0)
{

}

std::string SwitchOption::getName()
{
    return label;
}

void SwitchOption::setName(std::string text)
{
    label = text;
}

const std::deque<std::string> & SwitchOption::getTextItems() const
{
    return labels;
}

void SwitchOption::setTextItems(std::deque<std::string> text)
{
    labels = text;
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