#include "GroupOption.h"
#include <algorithm>

using namespace DiplomBukov;

GroupOption::GroupOption(bool vertical, const std::string & name)
    : label(name), vertical(vertical)
{
}

OptionPtr GroupOption::CreateCopy() const
{
    GroupOptionPtr ptr(new GroupOption(vertical));
    ptr->setName(getName());
    for (unsigned i = 0; i < optionList.size(); i++)
        ptr->addOption(optionList[i]->CreateCopy());
    return ptr;
}

const std::string & GroupOption::getName() const
{
    return label;
}

void GroupOption::setName(const std::string & name)
{
    label = name;
}

void GroupOption::visitMe(OptionWalkerPtr walker)
{
    walker->visit(GroupOptionPtr(this));
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

const std::deque<OptionPtr> & GroupOption::options()
{
    return optionList;
}

void GroupOption::addOption(const OptionPtr & option)
{
    optionList.push_back(option);
}

void GroupOption::removeOptions(const OptionPtr & option)
{
    std::remove(optionList.begin(), optionList.end(), option);
}