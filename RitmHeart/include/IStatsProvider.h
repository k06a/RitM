#ifndef ISTATISTICSPROVIDER_H
#define ISTATISTICSPROVIDER_H

#include <vector>
#include <string>
#include "CommonInclude.h"

namespace DiplomBukov
{
    class IStatsProvider
    {
    public:
        virtual ~IStatsProvider() {}

        virtual int getStatistic_size() const = 0;
        virtual i64 getStatistic_value(int i) const = 0;
        virtual const char * getStatistic_name(int i) const = 0;
    };
    // class IStatsProvider
}
// namespace DiplomBukov

#endif // ISTATISTICSPROVIDER_H