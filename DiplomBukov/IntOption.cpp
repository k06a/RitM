#include "IntOption.h"

using namespace DiplomBukov;

IntOption::IntOption(const std::string & name)
    : label(name), value(0), min(0), max(0)
{
}

IntOption::IntOption(i64 value, i64 min, i64 max,const std::string & name)
    : label(name), value(value), min(min), max(max)
{
}

IOptionPtr IntOption::CreateCopy() const
{
    IntOption * ptr = new IntOption(value, min, max);
    ptr->setName(getName());
    return IOptionPtr(ptr);
}

const std::string & IntOption::getName() const
{
    return label;
}

void IntOption::setName(const std::string & text)
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