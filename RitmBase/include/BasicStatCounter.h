#ifndef BASICSTATCOUNTER_H
#define BASICSTATCOUNTER_H

#include "IStatsProvider.h"

namespace RitM
{
    class RITM_API BasicStatCounter : public IStatsProvider
    {
    public:
        i64 i_count_in;
        i64 i_count_out;
        i64 i_count_loops_private, i_count_loops;

        BasicStatCounter();
        virtual int getStatistic_size() const;
        virtual i64 getStatistic_value(int i) const;
        virtual const char * getStatistic_name(int i) const;
    };
    // class BasicStatCounter

    typedef SharedPointer<BasicStatCounter>::Type BasicStatCounterPtr;
}
// namespace RitM

#endif // BASICSTATCOUNTER_H