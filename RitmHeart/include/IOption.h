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

        virtual const char * getName() const = 0;
        virtual void setName(const char * text) = 0;

        virtual void visitMe(OptionWalkerPtr walker) = 0;

        virtual std::string saveToString(int level = 0) = 0;
        virtual void loadFromString(std::string str, int level = 0) = 0;
    };
    // class IOption
}
// namespace DiplomBukov

#endif // IOPTION_H