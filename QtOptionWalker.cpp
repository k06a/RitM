#include <string>
#include <iostream>

#include <QVBoxLayout>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVariant>
#include <QFileDialog>

#include "QtOptionWalker.h"
#include "IProcessor.h"
#include "IAdapter.h"

using namespace DiplomBukov;

QtOptionWalker::QtOptionWalker()
    : QObject()
    , m_dialog(new QDialog)
    , m_layoutStack()
{
    QLayout * lay = new QVBoxLayout;
    m_dialog->setLayout(lay);
    m_layoutStack.append(lay);
}

void QtOptionWalker::clear()
{
    m_layoutStack.clear();
    delete m_dialog;

    m_dialog = new QDialog;
    QLayout * lay = new QVBoxLayout;
    m_dialog->setLayout(lay);
    m_layoutStack.append(lay);
}

QDialog * QtOptionWalker::dialog() const
{
    return m_dialog;
}

void QtOptionWalker::visit(AdapterPtr adap)
{
    OptionPtr opt = adap->getOptions();
    if (opt != NULL)
        opt->visitMe(shared_from_this());
}

void QtOptionWalker::visit(ProcessorPtr proc)
{
    OptionPtr opt = proc->getOptions();
    if (opt != NULL)
        opt->visitMe(shared_from_this());
}

void QtOptionWalker::visit(CheckOptionPtr opt)
{
    QCheckBox * checkBox = new QCheckBox(tr(opt->getName()), m_dialog);
    checkBox->setChecked(opt->isChecked());
    m_layoutStack.last()->addWidget(checkBox);
}

void QtOptionWalker::visit(SwitchOptionPtr opt)
{
    QWidget * gb = (opt->getName()[0] == 0)
                    ? new QWidget()
                    : new QGroupBox(tr(opt->getName()));
    QVBoxLayout * lay = new QVBoxLayout;
    for (int i = 0; i < opt->getTextItems_size(); i++)
    {
        QString str = opt->getTextItems_item(i);
        str.remove("   ");
        QRadioButton * rb = new QRadioButton(str);
        rb->setChecked(i == opt->getSelectedIndex());
        lay->addWidget(rb);
    }
    gb->setLayout(lay);

    m_layoutStack.last()->addWidget(gb);
}

void QtOptionWalker::visit(IntOptionPtr opt)
{
    QSpinBox * sb = new QSpinBox;
    sb->setMinimum(opt->minValue());
    sb->setMaximum(opt->maxValue());
    sb->setValue(opt->intValue());
    QLabel * lb = new QLabel;
    lb->setBuddy(sb);

    m_layoutStack.last()->addWidget(sb);
}

void QtOptionWalker::visit(TextLineOptionPtr opt)
{
    QLineEdit * le = new QLineEdit;
    le->setText(opt->getText());
    QLabel * lb = new QLabel(opt->getName());
    lb->setBuddy(le);

    m_layoutStack.last()->addWidget(le);
}

void QtOptionWalker::visit(FileOpenOptionPtr opt)
{
    QWidget * w = new QWidget;
    w->setLayout(new QHBoxLayout);

    QLineEdit * le = new QLineEdit;
    le->setText(opt->getFilename());
    QLabel * lb = new QLabel(tr(opt->getName()));
    lb->setBuddy(le);
    QPushButton * pb = new QPushButton(QObject::tr("Выбрать ..."));
    //QVariant extension = ;
    //QVariant fileLineEdit = ;
    pb->setProperty("extension", tr(opt->getExtension()));
    pb->setProperty("fileLineEdit", (int)le);

    connect(pb, SIGNAL(clicked()), this, SLOT(TextLineOption_buttonClicked()));

    w->layout()->addWidget(lb);
    w->layout()->addWidget(le);
    w->layout()->addWidget(pb);

    m_layoutStack.last()->addWidget(w);
}

void QtOptionWalker::visit(GroupOptionPtr opt)
{
    QWidget * gb = (opt->getName()[0] == 0)
                    ? new QWidget()
                    : new QGroupBox(opt->getName());
    QBoxLayout * lay = new QBoxLayout(
        opt->isVertical()
        ? QBoxLayout::TopToBottom
        : QBoxLayout::LeftToRight);
    m_layoutStack.append(lay);
    for (int i = 0; i < opt->options_size(); i++)
        opt->options_item(i)->visitMe(shared_from_this());
    gb->setLayout(lay);
    m_layoutStack.pop_back();

    m_layoutStack.last()->addWidget(gb);
}

void QtOptionWalker::visit(ListOptionPtr<OptionPtr,OptionPtr>::Type opt)
{
    opt->left->visitMe(shared_from_this());
    opt->right->visitMe(shared_from_this());
}

void QtOptionWalker::visit(OptionPtr opt)
{
    throw "Not Implemented";
}

void QtOptionWalker::TextLineOption_buttonClicked()
{
    QPushButton * pushButton = (QPushButton *)sender();
    QString ext = pushButton->property("extension").toString();
    int fileLineEdit_addr = pushButton->property("fileLineEdit").toInt();
    QLineEdit * lineEdit = (QLineEdit *)fileLineEdit_addr;

    QString str = QFileDialog::getOpenFileName(m_dialog, tr("Выбрать файл"), "", ext);
    if (!str.isEmpty())
        lineEdit->setText(str);
}