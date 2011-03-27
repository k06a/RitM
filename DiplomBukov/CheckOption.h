#ifndef CHECKOPTION_H
#define CHECKOPTION_H

#include <string>

#include "CommonInclude.h"
#include "IOption.h"
#include "IOptionWalker.h"

namespace DiplomBukov
{
    class CheckOption
        : public IOption
        , public EnableSmartFromThis<CheckOption>::Type
    {
        std::string label;
        bool value;

    public:
        CheckOption(bool checked = false, const std::string & name = "");
        virtual OptionPtr CreateCopy() const;

        virtual const std::string & getName() const;
        virtual void setName(const std::string & text);

        virtual void visitMe(OptionWalkerPtr walker);

        bool isChecked();
        void setChecked(bool checked);
    };
    // class CheckOption
}
// namespace DiplomBukov

#endif // CHECKOPTION_H