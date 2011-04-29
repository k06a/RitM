#ifndef QTOPTIONWALKER_H
#define QTOPTIONWALKER_H

#include <QObject>
#include <QDialog>

#include "CommonInclude.h"
#include "IOptionWalker.h"
#include "Options.h"

#include "CheckOption.h"
#include "SwitchOption.h"
#include "IntOption.h"
#include "GroupOption.h"
#include "TextLineOption.h"
#include "FileOpenOption.h"
#include "ListOption.h"

namespace DiplomBukov
{
    class QtOptionWalker
        : public QObject
        , public EnableSharedFromThis<QtOptionWalker>::Type
        , public IOptionWalker
    {
        Q_OBJECT

        QDialog * m_dialog;
        QList<QLayout*> m_layoutStack;

    public:
        QtOptionWalker();

        void clear();
        QDialog * dialog() const;

        virtual void visit(AdapterPtr adap);
        virtual void visit(ProcessorPtr proc);

        virtual void visit(CheckOptionPtr opt);
        virtual void visit(SwitchOptionPtr opt);
        virtual void visit(IntOptionPtr opt);
        virtual void visit(GroupOptionPtr opt);
        virtual void visit(TextLineOptionPtr opt);
        virtual void visit(FileOpenOptionPtr opt);

        virtual void visit(ListOptionPtr<OptionPtr,OptionPtr>::Type opt);

        virtual void visit(OptionPtr opt);

    public slots:
        void TextLineOption_buttonClicked();
    };
    // class QtOptionWalker

    typedef SharedPointer<QtOptionWalker>::Type QtOptionWalkerPtr;
}

#endif // QTOPTIONWALKER_H