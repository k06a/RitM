#ifndef QTOPTIONWALKER_H
#define QTOPTIONWALKER_H

#include <QObject>
#include <QDialog>

#include "CommonInclude.h"
#include "IOptionWalker.h"
#include "Options.h"

#include "CheckOption.h"
#include "SwitchOption.h"
#include "ComboOption.h"
#include "IntOption.h"
#include "GroupOption.h"
#include "TextLineOption.h"
#include "FileOpenOption.h"
#include "FileSaveOption.h"
#include "PushButtonOption.h"
#include "ListOption.h"

namespace RitM
{
    class QtOptionWalker
        : public QObject
        , public EnableSharedFromThis<QtOptionWalker>::Type
        , public IOptionWalker
    {
        Q_OBJECT

        QDialog * m_dialog;
        QList<QLayout*> m_layoutStack;
        bool m_locked;

    public:
        QtOptionWalker(bool locked = false);

        void clear();
        QDialog * dialog() const;

        virtual void visit(AdapterPtr adap);
        virtual void visit(ProcessorPtr proc);

        virtual void visit(CheckOptionPtr opt);
        virtual void visit(SwitchOptionPtr opt);
        virtual void visit(ComboOptionPtr opt);
        virtual void visit(IntOptionPtr opt);
        virtual void visit(GroupOptionPtr opt);
        virtual void visit(TextLineOptionPtr opt);
        virtual void visit(FileOpenOptionPtr opt);
        virtual void visit(FileSaveOptionPtr opt);
        virtual void visit(PushButtonOptionPtr opt);

        virtual void visit(ListOptionPtr<OptionPtr,OptionPtr>::Type opt);

        virtual void visit(OptionPtr opt);

        void visitFileOption(FileOpenOptionPtr opt, const char * member);

    public slots:
        void CheckOptionPtr_changed(int state);
        void SwitchOptionPtr_changed(bool checked);
        void ComboOptionPtr_currentChanged(int current);
        void IntOptionPtr_changed(int value);
        void TextLineOptionPtr_edited(QString text);
        void FileOptionPtr_edited(QString text);

        void FileOpenOption_buttonClicked();
        void FileSaveOption_buttonClicked();

        void PushButtonOptionPtr_clicked();
    };
    // class QtOptionWalker

    typedef SharedPointer<QtOptionWalker>::Type QtOptionWalkerPtr;
}

#endif // QTOPTIONWALKER_H