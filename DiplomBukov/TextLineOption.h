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
        : public IOption
        , public EnableSmartFromThis<TextLineOption>::Type
    {
        std::string label;
        std::string textline;

    public:
        TextLineOption(const std::string & text = "");
        virtual OptionPtr CreateCopy() const;

        virtual const std::string & getName() const;
        virtual void setName(const std::string & name);

        virtual void visitMe(OptionWalkerPtr walker);

        const std::string & getText() const;
        void setText(const std::string & name);
    };
    // class TextLineOption
}
// namespace DiplomBukov

#endif // TEXTLINEOPTION_H