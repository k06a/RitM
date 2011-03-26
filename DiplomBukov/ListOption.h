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

    protected:
        bool vertical;

    public:
        ListOption(const T1 & item1 = T1(),
                   const T2 & item2 = T2(),
                   bool vertical = true,
                   const std::string & name = "")
            : label(name), left(item1), right(item2), vertical(vertical)
        {
        }
        virtual IOptionPtr CreateCopy() const;

        virtual const std::string & getName() const;
        virtual void setName(const std::string & name);

        void makeHoizontal();
        void makeVertical();
        bool isHoizontal();
        bool isVertical();

    public:
        T1 left;
        T2 right;
    };
    // class ListOption

    template<typename T1, typename T2>
    struct ListOptionPtr
    {
        typedef typename SmartPointer<ListOption<T1,T2> >::Type Type;
    };
}
// namespace DiplomBukov

#endif // LISTOPTION_H