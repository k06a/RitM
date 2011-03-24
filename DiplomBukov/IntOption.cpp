#include "IntOption.h"

using namespace DiplomBukov;

IntOption::IntOption()
: label(""), value(false)
{
}

std::string IntOption::getName()
{
    return label;
}

void IntOption::setName(std::string text)
{
    label = text;
}

i64 IntOption::minValue()
{
    return min;
}

void IntOption::setMinValue(i64 val)
{
    min = val;
}

i64 IntOption::maxValue()
{
    return max;
}

void IntOption::setMaxValue(i64 val)
{
    max = val;
}

i64 IntOption::intValue()
{
    return value;
}

bool IntOption::setIntValue(i64 val)
{
    if ((min <= val) && (val <= max))
    {
        value = val;
        return true;
    }
    return false;
}