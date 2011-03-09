#include "CheckOption.h"

using namespace DiplomBukov;

CheckOption::CheckOption()
    : label(""), value(false)
{
}

std::string CheckOption::getName()
{
    return label;
}

void CheckOption::setName(std::string text)
{
    label = text;
}

bool CheckOption::isChecked()
{
    return value;
}

void CheckOption::setChecked(bool checked)
{
    value = checked;
}