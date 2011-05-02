#include "TextLineOption.h"

using namespace DiplomBukov;

TextLineOption::TextLineOption(const char * text)
    : label(""), textline(text)
{
}

OptionPtr TextLineOption::CreateCopy() const
{
    TextLineOptionPtr ptr(new TextLineOption(textline.c_str()));
    ptr->setName(getName());
    return ptr;
}

const char * TextLineOption::getName() const
{
    return label.c_str();
}

void TextLineOption::setName(const char * text)
{
    label = text;
}

void TextLineOption::visitMe(OptionWalkerPtr walker)
{
    walker->visit(shared_from_this());
}

const char * TextLineOption::getText() const
{
    return textline.c_str();
}

void TextLineOption::setText(const char * text)
{
    textline = text;
}

std::string TextLineOption::saveToString(int level)
{
    return textline;
}

void TextLineOption::loadFromString(std::string str, int level)
{
    textline = str;
}