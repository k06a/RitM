#ifndef SWITCHOPTION_H
#define SWITCHOPTION_H

#include <deque>
#include <string>

#include "CommonInclude.h"
#include "IOption.h"
#include "IOptionWalker.h"

namespace DiplomBukov
{
    class RITM_API SwitchOption
        : public EnableSharedFromThis<SwitchOption>::Type
        , public IOption
    {
        std::string label;
        std::deque<std::string> labels;
        int selected;

    public:
        SwitchOption();
        SwitchOption(const char * name);
        virtual OptionPtr CreateCopy() const;

        virtual const char * getName() const;
        virtual void setName(const char * text);

        virtual void visitMe(OptionWalkerPtr walker);

        int getTextItems_size() const;
        const char * getTextItems_item(int i) const;

        void addTextItem(const char * text);
        void removeTextItem(const char * text);
        int getIndexOf(const char * item);

        int getSelectedIndex() const;
        void setSelectedIndex(int index);
        const char * getSelectedText() const;

        virtual std::string saveToString(int level = 0);
        virtual void loadFromString(std::string str, int level = 0);
    };
    // class SwitchOption
}
// namespace DiplomBukov

#endif // SWITCHOPTION_H