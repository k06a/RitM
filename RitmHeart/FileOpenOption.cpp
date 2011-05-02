#include "FileOpenOption.h"

using namespace DiplomBukov;

FileOpenOption::FileOpenOption(const char * ext,
                               const char * label)
    : label(label), ext(ext), filename("")
{
}

OptionPtr FileOpenOption::CreateCopy() const
{
    FileOpenOptionPtr ptr(new FileOpenOption(ext.c_str(),label.c_str()));
    ptr->setFilename(getFilename());
    return ptr;
}

const char * FileOpenOption::getName() const
{
    return label.c_str();
}

void FileOpenOption::setName(const char * text)
{
    label = text;
}

void FileOpenOption::visitMe(OptionWalkerPtr walker)
{
    walker->visit(shared_from_this());
}

const char * FileOpenOption::getFilename() const
{
    return filename.c_str();
}

void FileOpenOption::setFilename(const char * text)
{
    filename = text;
}

const char * FileOpenOption::getExtension() const
{
    return ext.c_str();
}

void FileOpenOption::setExtension(const char * extension)
{
    ext = extension;
}

std::string FileOpenOption::saveToString(int level)
{
    return filename;
}

void FileOpenOption::loadFromString(std::string str, int level)
{
    filename = str;
}