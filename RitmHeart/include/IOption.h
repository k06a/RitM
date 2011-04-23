#ifndef IOPTION_H
#define IOPTION_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    class IOption
    {
    public:
        virtual ~IOption() {}
        virtual OptionPtr CreateCopy() const = 0;

        virtual const std::string & getName() const = 0;
        virtual void setName(const std::string & text) = 0;

        virtual void visitMe(OptionWalkerPtr walker) = 0;
    };
    // class IOption
}
// namespace DiplomBukov

#endif // IOPTION_H