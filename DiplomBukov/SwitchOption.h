#ifndef SWITCHOPTION_H
#define SWITCHOPTION_H

#include <deque>
#include <string>

#include "IOption.h"

namespace DiplomBukov
{
    class SwitchOption : public IOption
    {
        std::string label;
        std::deque<std::string> labels;
        int selected;

    public:
        SwitchOption();

        virtual std::string getName();
        virtual void setName(std::string text);

        const std::deque<std::string> & getTextItems() const;
        void setTextItems(std::deque<std::string> text);

        int getSelectedIndex() const;
        void setSelectedIndex(int index);
        std::string getSelectedText() const;
    };
    // class SwitchOption
}
// namespace DiplomBukov

#endif // SWITCHOPTION_H