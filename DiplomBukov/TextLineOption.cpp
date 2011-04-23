#include "TextLineOption.h"

using namespace DiplomBukov;

TextLineOption::TextLineOption(const std::string & text)
    : label(""), textline(text)
{
}

OptionPtr TextLineOption::CreateCopy() const
{
    TextLineOptionPtr ptr(new TextLineOption(textline));
    ptr->setName(getName());
    return ptr;
}

const std::string & TextLineOption::getName() const
{
    return label;
}

void TextLineOption::setName(const std::string & text)
{
    label = text;
}

void TextLineOption::visitMe(OptionWalkerPtr walker)
{
    walker->visit(shared_from_this());
}

const std::string & TextLineOption::getText() const
{
    return textline;
}

void TextLineOption::setText(const std::string & text)
{
    textline = text;
}
