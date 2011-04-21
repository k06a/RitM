#ifndef IntOption_H
#define IntOption_H

#include <string>

#include "CommonInclude.h"
#include "IOption.h"
#include "IOptionWalker.h"

namespace DiplomBukov
{
    class IntOption
        : public IOption
        , public EnableSharedFromThis<IntOption>::Type
    {
        std::string label;
        i64 value;
        i64 min;
        i64 max;
        
    public:
        IntOption(const std::string & name = "");
        IntOption(i64 value, i64 min, i64 max, const std::string & name = "");
        virtual OptionPtr CreateCopy() const;

        virtual const std::string & getName() const;
        virtual void setName(const std::string & text);

        virtual void visitMe(OptionWalkerPtr walker);

        i64 minValue();
        void setMinValue(i64 val);
        
        i64 maxValue();
        void setMaxValue(i64 val);
        
        i64 intValue();
        bool setIntValue(i64 val);
    };
    // class IntOption
}
// namespace DiplomBukov

#endif // IntOption_H