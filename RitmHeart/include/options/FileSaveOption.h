#ifndef FILESAVEOPTION_H
#define FILESAVEOPTION_H

#include "CommonInclude.h"
#include "FileOpenOption.h"

namespace DiplomBukov
{
    class RITM_API FileSaveOption : public FileOpenOption
    {
    public:
        FileSaveOption(const char * ext = "*.*",
                       const char * label = "");

        virtual void visitMe(OptionWalkerPtr walker);
    };
    // class FileSaveOption
}
// namespace DiplomBukov

#endif // FILESAVEOPTION_H