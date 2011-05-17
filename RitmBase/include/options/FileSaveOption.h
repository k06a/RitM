#ifndef FILESAVEOPTION_H
#define FILESAVEOPTION_H

#include "CommonInclude.h"
#include "FileOpenOption.h"

namespace RitM
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
// namespace RitM

#endif // FILESAVEOPTION_H