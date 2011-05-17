#include "CheckOption.h"

using namespace RitM;

CheckOption::CheckOption(bool checked, const char * name)
    : label(name), value(checked)
{
}

OptionPtr CheckOption::CreateCopy() const
{
    CheckOptionPtr ptr(new CheckOption(value));
    ptr->setName(getName());
    return ptr;
}

const char * CheckOption::getName() const
{
    return label.c_str();
}

void CheckOption::setName(const char * name)
{
    label = name;
}

void CheckOption::visitMe(OptionWalkerPtr walker)
{
    walker->visit(shared_from_this());
}

bool CheckOption::isChecked()
{
    return value;
}

void CheckOption::setChecked(bool checked)
{
    value = checked;
}

std::string CheckOption::saveToString(int level)
{
    return value ? "1" : "0";
}

void CheckOption::loadFromString(std::string str, int level)
{
    value = (str == "1");
}