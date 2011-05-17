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
#include <QComboBox>

#include "QtOptionWalker.h"
#include "IProcessor.h"
#include "IAdapter.h"
#include "Log.h"

using namespace RitM;

QtOptionWalker::QtOptionWalker(bool locked)
    : QObject()
    , m_dialog(NULL)
    , m_layoutStack()
    , m_locked(locked)
{
    clear();
}

void QtOptionWalker::clear()
{
    m_layoutStack.clear();

    if (m_dialog != NULL)
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

    checkBox->setProperty("option", (int)opt.get());
    connect(checkBox, SIGNAL(stateChanged(int)),
            this, SLOT(CheckOptionPtr_changed(int)));

    checkBox->setDisabled(m_locked);

    m_layoutStack.last()->addWidget(checkBox);
}

void QtOptionWalker::visit(SwitchOptionPtr opt)
{
    QWidget * gb = (opt->getName()[0] == 0)
                    ? new QWidget()
                    : new QGroupBox(tr(opt->getName()));
    QLayout * lay = new QVBoxLayout;
    for (int i = 0; i < opt->getTextItems_size(); i++)
    {
        QString str = opt->getTextItems_item(i);
        str.remove("   ");

        QRadioButton * radioButton = new QRadioButton(str);
        radioButton->setProperty("option", (int)opt.get());
        radioButton->setProperty("switch_id", i);
        radioButton->setChecked(i == opt->getSelectedIndex());
        connect(radioButton, SIGNAL(clicked(bool)),
                this, SLOT(SwitchOptionPtr_changed(bool)));

        radioButton->setDisabled(m_locked);

        lay->addWidget(radioButton);
    }
    gb->setLayout(lay);

    m_layoutStack.last()->addWidget(gb);
}

void QtOptionWalker::visit(ComboOptionPtr opt)
{
    QComboBox * comboBox = new QComboBox;
    for (int i = 0; i < opt->getTextItems_size(); i++)
    {
        QString str = opt->getTextItems_item(i);
        str.remove("   ");
        comboBox->addItem(str);
    }
    comboBox->setCurrentIndex(opt->getSelectedIndex());
    
    comboBox->setProperty("option", (int)opt.get());
    connect(comboBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(ComboOptionPtr_currentChanged(int)));

    comboBox->setDisabled(m_locked);

    m_layoutStack.last()->addWidget(comboBox);
}

void QtOptionWalker::visit(IntOptionPtr opt)
{
    QSpinBox * spinBox = new QSpinBox;
    spinBox->setMinimum(opt->minValue());
    spinBox->setMaximum(opt->maxValue());
    spinBox->setValue(opt->intValue());
    QLabel * label = new QLabel;
    label->setBuddy(spinBox);

    spinBox->setProperty("option", (int)opt.get());
    connect(spinBox, SIGNAL(valueChanged(int)),
            this, SLOT(IntOptionPtr_changed(int)));

    spinBox->setDisabled(m_locked);

    m_layoutStack.last()->addWidget(spinBox);
}

void QtOptionWalker::visit(TextLineOptionPtr opt)
{
    QLineEdit * lineEdit = new QLineEdit;
    lineEdit->setText(opt->getText());
    QLabel * label = new QLabel(opt->getName());
    label->setBuddy(lineEdit);

    lineEdit->setProperty("option", (int)opt.get());
    connect(lineEdit, SIGNAL(textEdited(QString)),
            this, SLOT(TextLineOptionPtr_edited(QString)));

    lineEdit->setDisabled(m_locked);

    m_layoutStack.last()->addWidget(lineEdit);
}

void QtOptionWalker::visit(FileOpenOptionPtr opt)
{
    visitFileOption(opt, SLOT(FileOpenOption_buttonClicked()));
}

void QtOptionWalker::visit(FileSaveOptionPtr opt)
{
    visitFileOption(opt, SLOT(FileSaveOption_buttonClicked()));
}

void QtOptionWalker::visit(PushButtonOptionPtr opt)
{
    QPushButton * pushButton = new QPushButton(QObject::tr(opt->getName()));
    pushButton->setProperty("option", (int)opt.get());
    connect(pushButton, SIGNAL(clicked()),
            this, SLOT(PushButtonOptionPtr_clicked()));
    m_layoutStack.last()->addWidget(pushButton);
}

void QtOptionWalker::visitFileOption(FileOpenOptionPtr opt, const char * member)
{
    QWidget * w = new QWidget;
    w->setLayout(new QHBoxLayout);

    QLineEdit * lineEdit = new QLineEdit;
    lineEdit->setText(opt->getFilename());
    QLabel * label = new QLabel(tr(opt->getName()));
    label->setBuddy(lineEdit);

    QPushButton * pushButton = new QPushButton(QObject::tr("Выбрать ..."));
    pushButton->setProperty("extension", tr(opt->getExtension()));
    pushButton->setProperty("fileLineEdit", (int)lineEdit);

    connect(pushButton, SIGNAL(clicked()), this, member);
    lineEdit->setProperty("option", (int)opt.get());
    connect(lineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(FileOptionPtr_edited(QString)));

    w->layout()->addWidget(label);
    w->layout()->addWidget(lineEdit);
    w->layout()->addWidget(pushButton);

    pushButton->setDisabled(m_locked);
    lineEdit->setDisabled(m_locked);

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
    LogLine() << "В обходчике опций QtOptionWalker не реализован обход этого типа опций" << '\n';
}

// Slots

void QtOptionWalker::CheckOptionPtr_changed(int state)
{
    QCheckBox * checkBox = (QCheckBox *)sender();
    int opt_addr = checkBox->property("option").toInt();
    CheckOption * opt = (CheckOption *)opt_addr;
    opt->setChecked(state == Qt::Checked);
}

void QtOptionWalker::SwitchOptionPtr_changed(bool checked)
{
    if (!checked)
        return;

    QRadioButton * radioButton = (QRadioButton *)sender();
    int opt_addr = radioButton->property("option").toInt();
    int opt_id = radioButton->property("switch_id").toInt();
    SwitchOption * opt = (SwitchOption *)opt_addr;
    opt->setSelectedIndex(opt_id);
}

void QtOptionWalker::ComboOptionPtr_currentChanged(int current)
{
    QComboBox * comboBox = (QComboBox *)sender();
    int opt_addr = comboBox->property("option").toInt();
    ComboOption * opt = (ComboOption *)opt_addr;
    opt->setSelectedIndex(current);
}

void QtOptionWalker::IntOptionPtr_changed(int value)
{
    QSpinBox * spinBox = (QSpinBox *)sender();
    int opt_addr = spinBox->property("option").toInt();
    IntOption * opt = (IntOption *)opt_addr;
    opt->setIntValue(value);
}

void QtOptionWalker::TextLineOptionPtr_edited(QString text)
{
    QLineEdit * lineEdit = (QLineEdit *)sender();
    int opt_addr = lineEdit->property("option").toInt();
    TextLineOption * opt = (TextLineOption *)opt_addr;
    opt->setText(text.toAscii().data());
}

void QtOptionWalker::FileOpenOption_buttonClicked()
{
    QPushButton * pushButton = (QPushButton *)sender();
    QString ext = pushButton->property("extension").toString();
    int fileLineEdit_addr = pushButton->property("fileLineEdit").toInt();
    QLineEdit * lineEdit = (QLineEdit *)fileLineEdit_addr;

    QString str = QFileDialog::getOpenFileName(m_dialog, tr("Выбрать файл"), "", ext);
    if (!str.isEmpty())
        lineEdit->setText(str);
}

void QtOptionWalker::FileOptionPtr_edited(QString text)
{
    QLineEdit * lineEdit = (QLineEdit *)sender();
    int opt_addr = lineEdit->property("option").toInt();
    FileOpenOption * opt = (FileOpenOption *)opt_addr;
    opt->setFilename(text.toAscii().data());
}

void QtOptionWalker::FileSaveOption_buttonClicked()
{
    QPushButton * pushButton = (QPushButton *)sender();
    QString ext = pushButton->property("extension").toString();
    int fileLineEdit_addr = pushButton->property("fileLineEdit").toInt();
    QLineEdit * lineEdit = (QLineEdit *)fileLineEdit_addr;

    QString str = QFileDialog::getSaveFileName(m_dialog, tr("Выбрать файл"), "", ext);
    if (!str.isEmpty())
        lineEdit->setText(str);
}

void QtOptionWalker::PushButtonOptionPtr_clicked()
{
    QPushButton * pushButton = (QPushButton *)sender();
    int opt_addr = pushButton->property("option").toInt();
    PushButtonOption * opt = (PushButtonOption *)opt_addr;
    opt->click();
}