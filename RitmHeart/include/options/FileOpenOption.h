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
        FileOpenOption(const std::string & ext = "*.*",
                       const std::string & label = "");
        virtual OptionPtr CreateCopy() const;

        virtual const std::string & getName() const;
        virtual void setName(const std::string & name);

        virtual void visitMe(OptionWalkerPtr walker);

        const std::string & getFilename() const;
        void setFilename(const std::string & name);
    };
    // class FileOpenOption
}
// namespace DiplomBukov

#endif // FILEOPENOPTION_H