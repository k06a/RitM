#include "FileSaveOption.h"

using namespace DiplomBukov;

FileSaveOption::FileSaveOption(const char * ext,
                               const char * label)
    : FileOpenOption(ext, label)
{
}

void FileSaveOption::visitMe(OptionWalkerPtr walker)
{
    FileSaveOptionPtr p = SharedPointerCast<FileSaveOption>(shared_from_this());
    walker->visit(p);
}