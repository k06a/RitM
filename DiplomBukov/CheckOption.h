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
        CheckOption();

        virtual std::string getName();
        virtual void setName(std::string text);

        bool isChecked();
        void setChecked(bool checked);
    };
    // class CheckOption
}
// namespace DiplomBukov

#endif // CHECKOPTION_H