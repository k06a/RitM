#ifndef COMBOOPTION_H
#define COMBOOPTION_H

#include "CommonInclude.h"
#include "SwitchOption.h"

namespace DiplomBukov
{
    class ComboOption : public SwitchOption
    {
    public:
        ComboOption();
        ComboOption(const char * name);

        virtual void visitMe(OptionWalkerPtr walker);
    };
    // class ComboOption
}
// namespace DiplomBukov

#endif // COMBOOPTION_H