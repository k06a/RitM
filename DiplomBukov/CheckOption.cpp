#include "CheckOption.h"

using namespace DiplomBukov;

CheckOption::CheckOption(bool checked, const std::string & name)
    : label(name), value(checked)
{
}

OptionPtr CheckOption::CreateCopy() const
{
    CheckOptionPtr ptr(new CheckOption(value));
    ptr->setName(getName());
    return ptr;
}

const std::string & CheckOption::getName() const
{
    return label;
}

void CheckOption::setName(const std::string & name)
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