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

void GroupOption::removeLastOption()
{
    optionList.pop_back();
}

std::string GroupOption::saveToString(int level)
{
    std::string delim = "(";
    for (int l = 0; l <= level; l++)
        delim += "@";
    delim += ")";

    std::string str;
    for (unsigned i = 0; i < optionList.size(); i++)
    {
        if (str.size() > 0)
            str += delim;
        str += optionList[i]->saveToString(level + 1);
    }
    return str;
}

void GroupOption::loadFromString(std::string str, int level)
{
    std::string delim = "(";
    for (int l = 0; l <= level; l++)
        delim += "@";
    delim += ")";

    unsigned i = 0;
    while (str.size() > 0 && i < optionList.size())
    {
        std::string::iterator it = std::search(str.begin(), str.end(),
                                               delim.begin(), delim.end());
        optionList[i]->loadFromString(str.substr(0,it-str.begin()), level+1);

        if (it == str.end())
            str.erase(str.begin(), str.end());
        else
            str.erase(str.begin(), it + delim.size());
        i++;
    }
}