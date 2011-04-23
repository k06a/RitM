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
    class FileOpenOption;
    class GroupOption;

    template<typename T1, typename T2>
    class ListOption;

    // ----------------------------------------------------------------

    typedef SharedPointer<CheckOption>::Type CheckOptionPtr;
    typedef SharedPointer<SwitchOption>::Type SwitchOptionPtr;
    typedef SharedPointer<IntOption>::Type IntOptionPtr;
    typedef SharedPointer<TextLineOption>::Type TextLineOptionPtr;
    typedef SharedPointer<FileOpenOption>::Type FileOpenOptionPtr;
    typedef SharedPointer<GroupOption>::Type GroupOptionPtr;

    template<typename T1, typename T2>
    struct ListOptionPtr
    {
        typedef typename SharedPointer<ListOption<T1,T2> >::Type Type;
    };

    // ----------------------------------------------------------------
}
// namespace DiplomBukov

#endif // OPTIONFORWARDDECLS_H
