#include "GroupOption.h"
#include <algorithm>

using namespace DiplomBukov;

GroupOption::GroupOption(bool vertical, const std::string & name)
    : label(name), vertical(vertical)
{
}

IOptionPtr GroupOption::CreateCopy() const
{
    GroupOption * ptr = new GroupOption(vertical);
    ptr->setName(getName());
    for (unsigned i = 0; i < optionList.size(); i++)
        ptr->addOptions(optionList[i]->CreateCopy());
    return IOptionPtr(ptr);
}

const std::string & GroupOption::getName() const
{
    return label;
}

void GroupOption::setName(const std::string & name)
{
    label = name;
}

void GroupOption::makeHoizontal()
{
    vertical = false;
}

void GroupOption::makeVertical()
{
    vertical = true;
}

bool GroupOption::isHoizontal()
{
    return !vertical;
}

bool GroupOption::isVertical()
{
    return vertical;
}

const std::deque<IOptionPtr> & GroupOption::options()
{
    return optionList;
}

void GroupOption::addOptions(IOptionPtr option)
{
    optionList.push_back(option);
}

void GroupOption::removeOptions(IOptionPtr option)
{
    std::remove(optionList.begin(), optionList.end(), option);
}