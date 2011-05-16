#ifndef OPTIONFORWARDDECLS_H
#define OPTIONFORWARDDECLS_H

#include "CommonInclude.h"

namespace DiplomBukov
{
    // ----------------------------------------------------------------

    class CheckOption;
    class SwitchOption;
    class ComboOption;
    class IntOption;
    class TextLineOption;
    class FileOpenOption;
    class FileSaveOption;
    class PushButtonOption;
    class IPushButtonOptionListener;
    class GroupOption;

    template<typename T1, typename T2>
    class ListOption;

    // ----------------------------------------------------------------

    typedef SharedPointer<CheckOption>::Type CheckOptionPtr;
    typedef SharedPointer<SwitchOption>::Type SwitchOptionPtr;
    typedef SharedPointer<ComboOption>::Type ComboOptionPtr;
    typedef SharedPointer<IntOption>::Type IntOptionPtr;
    typedef SharedPointer<TextLineOption>::Type TextLineOptionPtr;
    typedef SharedPointer<FileOpenOption>::Type FileOpenOptionPtr;
    typedef SharedPointer<FileSaveOption>::Type FileSaveOptionPtr;
    typedef SharedPointer<PushButtonOption>::Type PushButtonOptionPtr;
    typedef SharedPointer<IPushButtonOptionListener>::Type PushButtonOptionListenerPtr;
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
