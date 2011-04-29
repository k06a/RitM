#include "CheckOption.h"

using namespace DiplomBukov;

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