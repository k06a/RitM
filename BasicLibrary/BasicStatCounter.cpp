#include "BasicStatCounter.h"

using namespace DiplomBukov;

BasicStatCounter::BasicStatCounter()
    : i_count_in(0), i_count_out(0)
{
}

int BasicStatCounter::getStatistic_size() const
{
    return 2;
}

i64 BasicStatCounter::getStatistic_value(int i) const
{
    switch(i)
    {
        case 0: return i_count_in;
        case 1: return i_count_out;
    }
    return 0;
}

const char * BasicStatCounter::getStatistic_name(int i) const
{
    switch(i)
    {
        case 0: return "Кол-во пакетов полученных из среды";
        case 1: return "Кол-во пакетов переданных в среду";
    }
    return "";
}