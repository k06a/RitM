#ifndef CHECKOPTION_H
#define CHECKOPTION_H

#include <string>

namespace DiplomBukov
{
    class CheckOption
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