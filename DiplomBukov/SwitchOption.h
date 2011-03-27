#ifndef SWITCHOPTION_H
#define SWITCHOPTION_H

#include <deque>
#include <string>

#include "CommonInclude.h"
#include "IOption.h"
#include "IOptionWalker.h"

namespace DiplomBukov
{
    class SwitchOption
        : public IOption
        , public EnableSmartFromThis<SwitchOption>::Type
    {
        std::string label;
        std::deque<std::string> labels;
        int selected;

    public:
        SwitchOption();
        SwitchOption(const std::string & item,
                     const std::string & name = "");
        SwitchOption(const std::deque<std::string> & text,
                     const std::string & name = "");
        virtual OptionPtr CreateCopy() const;

        virtual const std::string & getName() const;
        virtual void setName(const std::string & text);

        virtual void visitMe(OptionWalkerPtr walker);

        const std::deque<std::string> & getTextItems() const;
        void setTextItems(const std::deque<std::string> & text);
        void addTextItem(const std::string & text);
        void removeTextItem(const std::string & text);
        int getIndexOf(const std::string & item);

        int getSelectedIndex() const;
        void setSelectedIndex(int index);
        std::string getSelectedText() const;
    };
    // class SwitchOption
}
// namespace DiplomBukov

#endif // SWITCHOPTION_H