#include <string>
#include <iostream>

#include "ConsoleOptionWalket.h"
#include "IProcessor.h"
#include "IAdapter.h"

using namespace DiplomBukov;

void ConsoleOptionWalker::visit(AdapterPtr adap)
{
    OptionPtr opt = adap->getOptions();
    if (opt != NULL)
    {
        std::cout << "[ Options of adapter \""
                  << adap->getProcessorName()
                  << "\" ]" << std::endl;
        opt->visitMe(this->shared_from_this());
        std::cout << std::endl;
    }
}

void ConsoleOptionWalker::visit(ProcessorPtr proc)
{
    OptionPtr opt = proc->getOptions();
    if (opt != NULL)
    {
        std::cout << "[ Options of processor \""
                  << proc->getProcessorName()
                  << "\" ]" << std::endl;
        opt->visitMe(this->shared_from_this());
        std::cout << std::endl;
    }
}

void ConsoleOptionWalker::visit(CheckOptionPtr opt)
{
    std::cout << opt->getName() << " [y/n]: ";
    std::string ans;
    std::cin >> ans;
    opt->setChecked((ans == "y") || (ans == "Y"));
}

void ConsoleOptionWalker::visit(SwitchOptionPtr opt)
{
    std::cout << opt->getName() << ":" << std::endl;
    for (unsigned i = 0; i < opt->getTextItems().size(); i++)
        std::cout << i+1 << ". " << opt->getTextItems()[i] << std::endl;

    unsigned ans;
    do 
    {
        std::cout << "Select one value (1-"
                  << opt->getTextItems().size()
                  << "): ";
        std::cin >> ans;
    } while ((ans < 1) || (ans > opt->getTextItems().size()));

    opt->setSelectedIndex(ans);
}

void ConsoleOptionWalker::visit(IntOptionPtr opt)
{
    i64 ans = 0;
    do 
    {
        std::cout << "Enter integer value for \""
                  << opt->getName() << "\" ("
                  << opt->minValue() << "-"
                  << opt->maxValue() << "): ";
        std::cin >> ans;
    } while ((ans < opt->minValue()) || (ans > opt->maxValue()));

    opt->setIntValue(ans);
}

void ConsoleOptionWalker::visit(TextLineOptionPtr opt)
{
    std::cout << "Enter string value for \""
              << opt->getName() << "\": ";
    std::string ans;
    std::getline(std::cin, ans, '\n');
    opt->setText(ans);
}

void ConsoleOptionWalker::visit(GroupOptionPtr opt)
{
    for (unsigned i = 0; i < opt->options().size(); i++)
        opt->options()[i]->visitMe(this->shared_from_this());
}

void ConsoleOptionWalker::visit(ListOptionPtr<OptionPtr,OptionPtr>::Type opt)
{
    opt->left->visitMe(this->shared_from_this());
    opt->right->visitMe(this->shared_from_this());
}

void ConsoleOptionWalker::visit(OptionPtr opt)
{
    throw "Not Implemented";
}