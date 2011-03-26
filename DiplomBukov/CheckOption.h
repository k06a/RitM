#ifndef CHECKOPTION_H
#define CHECKOPTION_H

#include <string>

#include "IOption.h"

namespace DiplomBukov
{
    class CheckOption : public IOption
    {
        std::string label;
        bool value;

    public:
        CheckOption(bool checked = false, const std::string & name = "");
        virtual OptionPtr CreateCopy() const;

        virtual const std::string & getName() const;
        virtual void setName(const std::string & text);

        bool isChecked();
        void setChecked(bool checked);
    };
    // class CheckOption

    typedef SmartPointer<CheckOption>::Type CheckOptionPtr;
}
// namespace DiplomBukov

#endif // CHECKOPTION_H