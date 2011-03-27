#ifndef IOPTIONWALKER_H
#define IOPTIONWALKER_H

#include "CommonInclude.h"
#include "CheckOption.h"
#include "SwitchOption.h"
#include "IntOption.h"
#include "GroupOption.h"
#include "ListOption.h"
#include "TextLineOption.h"

namespace DiplomBukov
{
    class IOptionWalker
    {
    public:
        virtual ~IOptionWalker() {}
        
        virtual void visit(ProcessorPtr proc) = 0;

        virtual void visit(CheckOptionPtr opt) = 0;
        virtual void visit(SwitchOptionPtr opt) = 0;
        virtual void visit(IntOptionPtr opt) = 0;
        virtual void visit(GroupOptionPtr opt) = 0;
        virtual void visit(TextLineOptionPtr opt) = 0;

        virtual void visit(ListOptionPtr<OptionPtr,OptionPtr> opt) = 0;

        virtual void visit(OptionPtr opt) = 0;
    };
    // class IOptionWalker
}
// namespace DiplomBukov

#endif // IOPTIONWALKER_H