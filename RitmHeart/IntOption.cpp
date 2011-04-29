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

OptionPtr IntOption::CreateCopy() const
{
    IntOptionPtr ptr(new IntOption(value, min, max));
    ptr->setName(getName());
    return ptr;
}

const char * IntOption::getName() const
{
    return label.c_str();
}

void IntOption::setName(const char * text)
{
    label = text;
}

void IntOption::visitMe(OptionWalkerPtr walker)
{
    walker->visit(shared_from_this());
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