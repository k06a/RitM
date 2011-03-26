#ifndef UNITEDOPTION_H
#define UNITEDOPTION_H

#include <string>
#include <deque>

#include "CommonInclude.h"
#include "IOption.h"

namespace DiplomBukov
{
    class GroupOption : public IOption
    {
        std::string label;
        std::deque<IOptionPtr> optionList;
        bool vertical;

    public:
        GroupOption(bool vertical = true, const std::string & name = "");
        virtual IOptionPtr CreateCopy() const;

        virtual const std::string & getName() const;
        virtual void setName(const std::string & name);

        void makeHoizontal();
        void makeVertical();
        bool isHoizontal();
        bool isVertical();

        const std::deque<IOptionPtr> & options();
        void addOptions(IOptionPtr option);
        void removeOptions(IOptionPtr option);
    };
    // class GroupOption
}
// namespace DiplomBukov

#endif // UNITEDOPTION_H