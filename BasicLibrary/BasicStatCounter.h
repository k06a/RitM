#ifndef BASICSTATCOUNTER_H
#define BASICSTATCOUNTER_H

#include "IStatsProvider.h"

using namespace RitM;

class BasicStatCounter : public IStatsProvider
{
public:
    i64 i_count_in;
    i64 i_count_out;

    BasicStatCounter();
    virtual int getStatistic_size() const;
    virtual i64 getStatistic_value(int i) const;
    virtual const char * getStatistic_name(int i) const;
};

typedef SharedPointer<BasicStatCounter>::Type BasicStatCounterPtr;

#endif // BASICSTATCOUNTER_H