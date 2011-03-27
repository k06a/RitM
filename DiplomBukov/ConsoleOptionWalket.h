#ifndef CONSOLEOPTIONWALKER_H
#define CONSOLEOPTIONWALKER_H

#include "CommonInclude.h"
#include "IOptionWalker.h"

namespace DiplomBukov
{
    class ConsoleOptionWalker
    {
    public:
        virtual ~ConsoleOptionWalker() {}

        virtual void visit(ProcessorPtr proc);

        virtual void visit(CheckOptionPtr opt);
        virtual void visit(SwitchOptionPtr opt);
        virtual void visit(IntOptionPtr opt);
        virtual void visit(GroupOptionPtr opt);
        virtual void visit(TextLineOptionPtr opt);
        
        virtual void visit(ListOptionPtr<OptionPtr,OptionPtr> opt);

        virtual void visit(OptionPtr opt);
    };
    // class ConsoleOptionWalker

    typedef SmartPointer<ConsoleOptionWalker>::Type ConsoleOptionWalkerPtr;
}

#endif // CONSOLEOPTIONWALKER_H