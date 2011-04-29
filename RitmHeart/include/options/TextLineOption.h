#ifndef TEXTLINEOPTION_H
#define TEXTLINEOPTION_H

#include <deque>
#include <string>

#include "CommonInclude.h"
#include "IOption.h"
#include "IOptionWalker.h"

namespace DiplomBukov
{
    class TextLineOption
        : public EnableSharedFromThis<TextLineOption>::Type
        , public IOption
    {
        std::string label;
        std::string textline;

    public:
        TextLineOption(const char * text = "");
        virtual OptionPtr CreateCopy() const;

        virtual const char * getName() const;
        virtual void setName(const char * name);

        virtual void visitMe(OptionWalkerPtr walker);

        const char * getText() const;
        void setText(const char * name);
    };
    // class TextLineOption
}
// namespace DiplomBukov

#endif // TEXTLINEOPTION_H