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

OptionPtr ComboOption::CreateCopy() const
{
    SwitchOptionPtr ptr(new ComboOption(getName()));
    for (int i = 0; i < getTextItems_size(); i++)
        ptr->addTextItem(getTextItems_item(i));
    return ptr;
}

void ComboOption::visitMe(OptionWalkerPtr walker)
{
    ComboOptionPtr p = SharedPointerCast<ComboOption>(shared_from_this());
    walker->visit(p);
}