#ifndef IOPTIONWALKER_H
#define IOPTIONWALKER_H

#include "CommonInclude.h"
#include "Options.h"

namespace RitM
{
    class IOptionWalker
    {
    public:
        virtual ~IOptionWalker() {}
        
        virtual void visit(AdapterPtr adap) = 0;
        virtual void visit(ProcessorPtr proc) = 0;
        
        virtual void visit(CheckOptionPtr opt) = 0;
        virtual void visit(SwitchOptionPtr opt) = 0;
        virtual void visit(ComboOptionPtr opt) = 0;
        virtual void visit(IntOptionPtr opt) = 0;
        virtual void visit(GroupOptionPtr opt) = 0;
        virtual void visit(TextLineOptionPtr opt) = 0;
        virtual void visit(FileOpenOptionPtr opt) = 0;
        virtual void visit(FileSaveOptionPtr opt) = 0;
        virtual void visit(PushButtonOptionPtr opt) = 0;

        virtual void visit(ListOptionPtr<OptionPtr,OptionPtr>::Type opt) = 0;

        virtual void visit(OptionPtr opt) = 0;
    };
    // class IOptionWalker
}
// namespace RitM

#endif // IOPTIONWALKER_H