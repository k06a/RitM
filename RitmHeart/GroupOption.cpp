#include "GroupOption.h"
#include <algorithm>

using namespace DiplomBukov;

GroupOption::GroupOption(bool vertical, const char * name)
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

const char * GroupOption::getName() const
{
    return label.c_str();
}

void GroupOption::setName(const char * name)
{
    label = name;
}

void GroupOption::visitMe(OptionWalkerPtr walker)
{
    walker->visit(shared_from_this());
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

int GroupOption::options_size() const
{
    return optionList.size();
}

OptionPtr GroupOption::options_item(int i) const
{
    return optionList[i];
}

void GroupOption::addOption(OptionPtr option)
{
    optionList.push_back(option);
}

void GroupOption::removeOptions(OptionPtr option)
{
    std::remove(optionList.begin(), optionList.end(), option);
}