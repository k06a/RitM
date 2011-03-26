#include "TextLineOption.h"

using namespace DiplomBukov;

TextLineOption::TextLineOption(const std::string & text)
    : label(""), textline(text)
{
}

IOptionPtr TextLineOption::CreateCopy() const
{
    TextLineOption * ptr = new TextLineOption(textline);
    ptr->setName(getName());
    return IOptionPtr(ptr);
}

const std::string & TextLineOption::getName() const
{
    return label;
}

void TextLineOption::setName(const std::string & text)
{
    label = text;
}

const std::string & TextLineOption::getText() const
{
    return textline;
}

void TextLineOption::setText(const std::string & text)
{
    textline = text;
}
