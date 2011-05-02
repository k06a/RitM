#ifndef FILEOPENOPTION_H
#define FILEOPENOPTION_H

#include <deque>
#include <string>

#include "CommonInclude.h"
#include "IOption.h"
#include "IOptionWalker.h"

namespace DiplomBukov
{
    class FileOpenOption
        : public EnableSharedFromThis<FileOpenOption>::Type
        , public IOption
    {
        std::string ext;
        std::string label;
        std::string filename;

    public:
        FileOpenOption(const char * ext = "*.*",
                       const char * label = "");
        virtual OptionPtr CreateCopy() const;

        virtual const char * getName() const;
        virtual void setName(const char * name);

        virtual void visitMe(OptionWalkerPtr walker);

        const char * getFilename() const;
        void setFilename(const char * name);

        const char * getExtension() const;
        void setExtension(const char * extension);

        virtual std::string saveToString(int level = 0);
        virtual void loadFromString(std::string str, int level = 0);
    };
    // class FileOpenOption
}
// namespace DiplomBukov

#endif // FILEOPENOPTION_H