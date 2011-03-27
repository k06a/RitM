#ifndef OPTIONFORWARDDECLS_H
#define OPTIONFORWARDDECLS_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    // ----------------------------------------------------------------

    class CheckOption;
    class SwitchOption;
    class IntOption;
    class TextLineOption;
    class GroupOption;

    template<typename T1, typename T2>
    class ListOption;

    // ----------------------------------------------------------------

    typedef SmartPointer<CheckOption>::Type CheckOptionPtr;
    typedef SmartPointer<SwitchOption>::Type SwitchOptionPtr;
    typedef SmartPointer<IntOption>::Type IntOptionPtr;
    typedef SmartPointer<TextLineOption>::Type TextLineOptionPtr;
    typedef SmartPointer<GroupOption>::Type GroupOptionPtr;

    template<typename T1, typename T2>
    struct ListOptionPtr
    {
        typedef typename SmartPointer<ListOption<T1,T2> >::Type Type;
    };

    // ----------------------------------------------------------------
}
// namespace DiplomBukov

#endif // OPTIONFORWARDDECLS_H
