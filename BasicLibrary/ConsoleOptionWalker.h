#ifndef CONSOLEOPTIONWALKER_H
#define CONSOLEOPTIONWALKER_H

#include "CommonInclude.h"
#include "IOptionWalker.h"
#include "Options.h"

#include "CheckOption.h"
#include "SwitchOption.h"
#include "ComboOption.h"
#include "IntOption.h"
#include "GroupOption.h"
#include "TextLineOption.h"
#include "FileOpenOption.h"
#include "FileSaveOption.h"
#include "PushButtonOption.h"
#include "ListOption.h"

namespace RitM
{
    class ConsoleOptionWalker
        : public EnableSharedFromThis<ConsoleOptionWalker>::Type
        , public IOptionWalker
    {
    public:
        ConsoleOptionWalker();

        virtual void visit(AdapterPtr adap);
        virtual void visit(ProcessorPtr proc);

        virtual void visit(CheckOptionPtr opt);
        virtual void visit(SwitchOptionPtr opt);
        virtual void visit(ComboOptionPtr opt);
        virtual void visit(IntOptionPtr opt);
        virtual void visit(GroupOptionPtr opt);
        virtual void visit(TextLineOptionPtr opt);
        virtual void visit(FileOpenOptionPtr opt);
        virtual void visit(FileSaveOptionPtr opt);
        virtual void visit(PushButtonOptionPtr opt);

        virtual void visit(ListOptionPtr<OptionPtr,OptionPtr>::Type opt);

        virtual void visit(OptionPtr opt);
    };
    // class ConsoleOptionWalker

    typedef SharedPointer<ConsoleOptionWalker>::Type ConsoleOptionWalkerPtr;
}

#endif // CONSOLEOPTIONWALKER_H