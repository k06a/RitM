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
        : public IOption
        , public EnableSharedFromThis<GroupOption>::Type
    {
        std::string label;
        std::deque<OptionPtr> optionList;
        bool vertical;

    public:
        GroupOption(bool vertical = true, const std::string & name = "");
        virtual OptionPtr CreateCopy() const;

        virtual const std::string & getName() const;
        virtual void setName(const std::string & name);

        virtual void visitMe(OptionWalkerPtr walker);

        void makeHoizontal();
        void makeVertical();
        bool isHoizontal();
        bool isVertical();

        const std::deque<OptionPtr> & options();
        void addOption(const OptionPtr & option);
        void removeOptions(const OptionPtr & option);
    };
    // class GroupOption
}
// namespace DiplomBukov

#endif // UNITEDOPTION_H