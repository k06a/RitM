#ifndef CHECKOPTION_H
#define CHECKOPTION_H

#include <string>

#include "CommonInclude.h"
#include "IOption.h"
#include "IOptionWalker.h"

namespace RitM
{
    class RITM_API CheckOption
        : public EnableSharedFromThis<CheckOption>::Type
        , public IOption
    {
        std::string label;
        bool value;

    public:
        CheckOption(bool checked = false, const char * name = "");
        virtual OptionPtr CreateCopy() const;

        virtual const char * getName() const;
        virtual void setName(const char * text);

        virtual void visitMe(OptionWalkerPtr walker);

        bool isChecked();
        void setChecked(bool checked);

        virtual std::string saveToString(int level);
        virtual void loadFromString(std::string str, int level = 0);
    };
    // class CheckOption
}
// namespace RitM

#endif // CHECKOPTION_H