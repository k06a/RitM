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

        virtual const std::vector<std::string> & getStatisticNames() const = 0;
        virtual const std::vector<i64> & getStatisticValues() const = 0;
    };
    // class IStatsProvider
}
// namespace DiplomBukov

#endif // ISTATISTICSPROVIDER_H