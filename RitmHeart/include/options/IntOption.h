#ifndef IntOption_H
#define IntOption_H

#include <string>

#include "CommonInclude.h"
#include "IOption.h"
#include "IOptionWalker.h"

namespace RitM
{
    class RITM_API IntOption
        : public EnableSharedFromThis<IntOption>::Type
        , public IOption
    {
        std::string label;
        i64 value;
        i64 min;
        i64 max;
        
    public:
        IntOption(const std::string & name = "");
        IntOption(i64 value, i64 min, i64 max, const std::string & name = "");
        virtual OptionPtr CreateCopy() const;

        virtual const char * getName() const;
        virtual void setName(const char * text);

        virtual void visitMe(OptionWalkerPtr walker);

        i64 minValue();
        void setMinValue(i64 val);
        
        i64 maxValue();
        void setMaxValue(i64 val);
        
        i64 intValue();
        bool setIntValue(i64 val);

        virtual std::string saveToString(int level = 0);
        virtual void loadFromString(std::string str, int level = 0);
    };
    // class IntOption
}
// namespace RitM

#endif // IntOption_H