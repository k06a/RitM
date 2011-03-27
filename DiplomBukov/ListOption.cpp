#include "ListOption.h"
#include <algorithm>

using namespace DiplomBukov;
/*
template<typename T1, typename T2>
ListOption<T1,T2>::ListOption(bool vertical,
                              const std::string & name)
    : label(name), vertical(vertical)
{
}


template<typename T1, typename T2>
ListOption<T1,T2>::ListOption(const T1 & item1,
                              const T2 & item2,
                              bool vertical,
                              const std::string & name)
    : label(name), first(item1), second(item2), vertical(vertical)
{
}
*/
template<typename T1, typename T2>
OptionPtr ListOption<T1,T2>::CreateCopy() const
{
    ListOptionPtr<T1,T2>::Type ptr(new ListOption(labels));
    ptr->setName(getName());
    return ptr;
}

template<typename T1, typename T2>
const std::string & ListOption<T1,T2>::getName() const
{
    return label;
}

template<typename T1, typename T2>
void ListOption<T1,T2>::setName(const std::string & text)
{
    label = text;
}

template<typename T1, typename T2>
void ListOption<T1,T2>::visitMe(OptionWalkerPtr walker)
{
    walker->visit(this->shared_from_this());
}

template<typename T1, typename T2>
void ListOption<T1,T2>::makeHoizontal()
{
    vertical = false;
}

template<typename T1, typename T2>
void ListOption<T1,T2>::makeVertical()
{
    vertical = true;
}

template<typename T1, typename T2>
bool ListOption<T1,T2>::isHoizontal()
{
    return !vertical;
}

template<typename T1, typename T2>
bool ListOption<T1,T2>::isVertical()
{
    return vertical;
}
