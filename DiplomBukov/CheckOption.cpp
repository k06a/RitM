#include "CheckOption.h"

using namespace DiplomBukov;

CheckOption::CheckOption(bool checked, const std::string & name)
    : label(name), value(checked)
{
}

IOptionPtr CheckOption::CreateCopy() const
{
    CheckOption * ptr = new CheckOption(value);
    ptr->setName(getName());
    return IOptionPtr(ptr);
}

const std::string & CheckOption::getName() const
{
    return label;
}

void CheckOption::setName(const std::string & name)
{
    label = name;
}

bool CheckOption::isChecked()
{
    return value;
}

void CheckOption::setChecked(bool checked)
{
    value = checked;
}