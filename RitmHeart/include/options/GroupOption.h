#ifndef UNITEDOPTION_H
#define UNITEDOPTION_H

#include <string>
#include <deque>

#include "CommonInclude.h"
#include "IOption.h"
#include "IOptionWalker.h"

namespace DiplomBukov
{
    class GroupOption
        : public EnableSharedFromThis<GroupOption>::Type
        , public IOption
    {
        std::string label;
        std::deque<OptionPtr> optionList;
        bool vertical;

    public:
        GroupOption(bool vertical = true, const char * name = "");
        virtual OptionPtr CreateCopy() const;

        virtual const char * getName() const;
        virtual void setName(const char * name);

        virtual void visitMe(OptionWalkerPtr walker);

        void makeHoizontal();
        void makeVertical();
        bool isHoizontal();
        bool isVertical();

        int options_size() const;
        OptionPtr options_item(int i) const;
        void addOption(OptionPtr option);
        void removeOptions(OptionPtr option);

        virtual std::string saveToString(int level = 0);
        virtual void loadFromString(std::string str, int level = 0);
    };
    // class GroupOption
}
// namespace DiplomBukov

#endif // UNITEDOPTION_H