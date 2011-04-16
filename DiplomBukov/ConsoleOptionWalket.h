#ifndef CONSOLEOPTIONWALKER_H
#define CONSOLEOPTIONWALKER_H

#include "CommonInclude.h"
#include "IOptionWalker.h"
#include "OptionsForwardDecls.h"

#include "CheckOption.h"
#include "SwitchOption.h"
#include "IntOption.h"
#include "GroupOption.h"
#include "TextLineOption.h"
#include "ListOption.h"

namespace DiplomBukov
{
    class ConsoleOptionWalker
        : public IOptionWalker
        , public EnableSmartFromThis<ConsoleOptionWalker>::Type
    {
    public:
        virtual ~ConsoleOptionWalker() {}

        virtual void visit(AdapterPtr adap);
        virtual void visit(ProcessorPtr proc);

        virtual void visit(CheckOptionPtr opt);
        virtual void visit(SwitchOptionPtr opt);
        virtual void visit(IntOptionPtr opt);
        virtual void visit(GroupOptionPtr opt);
        virtual void visit(TextLineOptionPtr opt);
        
        virtual void visit(ListOptionPtr<OptionPtr,OptionPtr>::Type opt);

        virtual void visit(OptionPtr opt);
    };
    // class ConsoleOptionWalker

    typedef SmartPointer<ConsoleOptionWalker>::Type ConsoleOptionWalkerPtr;
}

#endif // CONSOLEOPTIONWALKER_H