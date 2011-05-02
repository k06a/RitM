#include "ComboOption.h"

using namespace DiplomBukov;

ComboOption::ComboOption()
    : SwitchOption()
{
}

ComboOption::ComboOption(const char * name)
    : SwitchOption(name)
{
}

void ComboOption::visitMe(OptionWalkerPtr walker)
{
    ComboOptionPtr p = SharedPointerCast<ComboOption>(shared_from_this());
    walker->visit(p);
}