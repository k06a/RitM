#ifndef COMBOOPTION_H
#define COMBOOPTION_H

#include "CommonInclude.h"
#include "SwitchOption.h"

namespace DiplomBukov
{
    class RITM_API ComboOption : public SwitchOption
    {
    public:
        ComboOption();
        ComboOption(const char * name);
        virtual OptionPtr CreateCopy() const;

        virtual void visitMe(OptionWalkerPtr walker);
    };
    // class ComboOption
}
// namespace DiplomBukov

#endif // COMBOOPTION_H