#ifndef LISTOPTION_H
#define LISTOPTION_H

#include <string>
#include <deque>

#include "CommonInclude.h"
#include "IOption.h"

namespace DiplomBukov
{
    template<typename T1, typename T2>
    class ListOption : public IOption
    {
        std::string label;
        T1 * first;
        T2 * second;
        bool vertical;

    public:
        ListOption(bool vertical = true, const std::string & name = "");
        virtual IOptionPtr CreateCopy() const;

        virtual const std::string & getName() const;
        virtual void setName(const std::string & name);

        void makeHoizontal();
        void makeVertical();
        bool isHoizontal();
        bool isVertical();

        T1 * left() const;
        void setLeft(IOption * option);
        
        T2 * right() const;
        void setRight(IOption * option);

        template<typename T>
        IOption * at(int index);
    };
    // class ListOption
}
// namespace DiplomBukov

#endif // LISTOPTION_H