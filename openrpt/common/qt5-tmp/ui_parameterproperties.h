/********************************************************************************
** Form generated from reading UI file 'parameterproperties.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAMETERPROPERTIES_H
#define UI_PARAMETERPROPERTIES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ParameterProperties
{
public:
    QGridLayout *gridLayout;
    QLabel *_typeLit;
    QComboBox *_type;
    QDialogButtonBox *buttonBox;
    QLabel *_nameLit;
    QLineEdit *_name;
    QStackedWidget *_stack;
    QWidget *_stringPage;
    QFormLayout *formLayout;
    QLabel *_stringLit;
    QLineEdit *_string;
    QWidget *_booleanPage;
    QFormLayout *formLayout_2;
    QLabel *_boolLit;
    QComboBox *_bool;
    QWidget *_integerPage;
    QFormLayout *formLayout_3;
    QLabel *_intLit;
    QLineEdit *_int;
    QWidget *_doublePage;
    QFormLayout *formLayout_4;
    QLabel *_doubleLit;
    QLineEdit *_double;
    QWidget *_listPage;
    QGridLayout *gridLayout_2;
    QLabel *_listLit;
    QListWidget *_list;
    QVBoxLayout *_listButtonLyt;
    QPushButton *_listNew;
    QFrame *line1_3;
    QPushButton *_listUp;
    QPushButton *_listEdit;
    QPushButton *_listDown;
    QFrame *line1_4;
    QPushButton *_listDelete;
    QSpacerItem *spacer3_2;
    QCheckBox *_active;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *ParameterProperties)
    {
        if (ParameterProperties->objectName().isEmpty())
            ParameterProperties->setObjectName(QStringLiteral("ParameterProperties"));
        ParameterProperties->resize(414, 372);
        gridLayout = new QGridLayout(ParameterProperties);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        _typeLit = new QLabel(ParameterProperties);
        _typeLit->setObjectName(QStringLiteral("_typeLit"));
        _typeLit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(_typeLit, 0, 0, 1, 1);

        _type = new QComboBox(ParameterProperties);
        _type->setObjectName(QStringLiteral("_type"));

        gridLayout->addWidget(_type, 0, 1, 1, 1);

        buttonBox = new QDialogButtonBox(ParameterProperties);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);

        gridLayout->addWidget(buttonBox, 0, 3, 2, 1);

        _nameLit = new QLabel(ParameterProperties);
        _nameLit->setObjectName(QStringLiteral("_nameLit"));
        _nameLit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(_nameLit, 1, 0, 1, 1);

        _name = new QLineEdit(ParameterProperties);
        _name->setObjectName(QStringLiteral("_name"));

        gridLayout->addWidget(_name, 1, 1, 1, 2);

        _stack = new QStackedWidget(ParameterProperties);
        _stack->setObjectName(QStringLiteral("_stack"));
        _stringPage = new QWidget();
        _stringPage->setObjectName(QStringLiteral("_stringPage"));
        formLayout = new QFormLayout(_stringPage);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        _stringLit = new QLabel(_stringPage);
        _stringLit->setObjectName(QStringLiteral("_stringLit"));
        _stringLit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, _stringLit);

        _string = new QLineEdit(_stringPage);
        _string->setObjectName(QStringLiteral("_string"));

        formLayout->setWidget(0, QFormLayout::FieldRole, _string);

        _stack->addWidget(_stringPage);
        _booleanPage = new QWidget();
        _booleanPage->setObjectName(QStringLiteral("_booleanPage"));
        formLayout_2 = new QFormLayout(_booleanPage);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        _boolLit = new QLabel(_booleanPage);
        _boolLit->setObjectName(QStringLiteral("_boolLit"));
        _boolLit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, _boolLit);

        _bool = new QComboBox(_booleanPage);
        _bool->setObjectName(QStringLiteral("_bool"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(_bool->sizePolicy().hasHeightForWidth());
        _bool->setSizePolicy(sizePolicy);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, _bool);

        _stack->addWidget(_booleanPage);
        _integerPage = new QWidget();
        _integerPage->setObjectName(QStringLiteral("_integerPage"));
        formLayout_3 = new QFormLayout(_integerPage);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
        _intLit = new QLabel(_integerPage);
        _intLit->setObjectName(QStringLiteral("_intLit"));
        _intLit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, _intLit);

        _int = new QLineEdit(_integerPage);
        _int->setObjectName(QStringLiteral("_int"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, _int);

        _stack->addWidget(_integerPage);
        _doublePage = new QWidget();
        _doublePage->setObjectName(QStringLiteral("_doublePage"));
        formLayout_4 = new QFormLayout(_doublePage);
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        _doubleLit = new QLabel(_doublePage);
        _doubleLit->setObjectName(QStringLiteral("_doubleLit"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, _doubleLit);

        _double = new QLineEdit(_doublePage);
        _double->setObjectName(QStringLiteral("_double"));

        formLayout_4->setWidget(0, QFormLayout::FieldRole, _double);

        _stack->addWidget(_doublePage);
        _listPage = new QWidget();
        _listPage->setObjectName(QStringLiteral("_listPage"));
        gridLayout_2 = new QGridLayout(_listPage);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        _listLit = new QLabel(_listPage);
        _listLit->setObjectName(QStringLiteral("_listLit"));
        _listLit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_2->addWidget(_listLit, 0, 0, 1, 1);

        _list = new QListWidget(_listPage);
        _list->setObjectName(QStringLiteral("_list"));

        gridLayout_2->addWidget(_list, 1, 0, 1, 1);

        _listButtonLyt = new QVBoxLayout();
        _listButtonLyt->setObjectName(QStringLiteral("_listButtonLyt"));
        _listNew = new QPushButton(_listPage);
        _listNew->setObjectName(QStringLiteral("_listNew"));

        _listButtonLyt->addWidget(_listNew);

        line1_3 = new QFrame(_listPage);
        line1_3->setObjectName(QStringLiteral("line1_3"));
        line1_3->setFrameShape(QFrame::HLine);
        line1_3->setFrameShadow(QFrame::Sunken);

        _listButtonLyt->addWidget(line1_3);

        _listUp = new QPushButton(_listPage);
        _listUp->setObjectName(QStringLiteral("_listUp"));

        _listButtonLyt->addWidget(_listUp);

        _listEdit = new QPushButton(_listPage);
        _listEdit->setObjectName(QStringLiteral("_listEdit"));

        _listButtonLyt->addWidget(_listEdit);

        _listDown = new QPushButton(_listPage);
        _listDown->setObjectName(QStringLiteral("_listDown"));

        _listButtonLyt->addWidget(_listDown);

        line1_4 = new QFrame(_listPage);
        line1_4->setObjectName(QStringLiteral("line1_4"));
        line1_4->setFrameShape(QFrame::HLine);
        line1_4->setFrameShadow(QFrame::Sunken);

        _listButtonLyt->addWidget(line1_4);

        _listDelete = new QPushButton(_listPage);
        _listDelete->setObjectName(QStringLiteral("_listDelete"));

        _listButtonLyt->addWidget(_listDelete);

        spacer3_2 = new QSpacerItem(20, 24, QSizePolicy::Minimum, QSizePolicy::Expanding);

        _listButtonLyt->addItem(spacer3_2);


        gridLayout_2->addLayout(_listButtonLyt, 1, 1, 1, 1);

        _stack->addWidget(_listPage);

        gridLayout->addWidget(_stack, 3, 0, 1, 4);

        _active = new QCheckBox(ParameterProperties);
        _active->setObjectName(QStringLiteral("_active"));

        gridLayout->addWidget(_active, 2, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

#ifndef QT_NO_SHORTCUT
        _typeLit->setBuddy(_type);
        _nameLit->setBuddy(_name);
        _stringLit->setBuddy(_string);
        _boolLit->setBuddy(_bool);
        _intLit->setBuddy(_int);
        _doubleLit->setBuddy(_double);
        _listLit->setBuddy(_list);
#endif // QT_NO_SHORTCUT

        retranslateUi(ParameterProperties);
        QObject::connect(_type, SIGNAL(currentIndexChanged(int)), _stack, SLOT(setCurrentIndex(int)));
        QObject::connect(buttonBox, SIGNAL(accepted()), ParameterProperties, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ParameterProperties, SLOT(reject()));

        _stack->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ParameterProperties);
    } // setupUi

    void retranslateUi(QDialog *ParameterProperties)
    {
        ParameterProperties->setWindowTitle(QApplication::translate("ParameterProperties", "Parameter Properties", 0));
        _typeLit->setText(QApplication::translate("ParameterProperties", "Type:", 0));
        _type->clear();
        _type->insertItems(0, QStringList()
         << QApplication::translate("ParameterProperties", "String", 0)
         << QApplication::translate("ParameterProperties", "Bool", 0)
         << QApplication::translate("ParameterProperties", "Int", 0)
         << QApplication::translate("ParameterProperties", "Double", 0)
         << QApplication::translate("ParameterProperties", "List", 0)
        );
        _nameLit->setText(QApplication::translate("ParameterProperties", "Name:", 0));
        _stringLit->setText(QApplication::translate("ParameterProperties", "Value:", 0));
        _boolLit->setText(QApplication::translate("ParameterProperties", "Value:", 0));
        _bool->clear();
        _bool->insertItems(0, QStringList()
         << QApplication::translate("ParameterProperties", "False", 0)
         << QApplication::translate("ParameterProperties", "True", 0)
        );
        _intLit->setText(QApplication::translate("ParameterProperties", "Value:", 0));
        _doubleLit->setText(QApplication::translate("ParameterProperties", "Value:", 0));
        _listLit->setText(QApplication::translate("ParameterProperties", "Values:", 0));
        _listNew->setText(QApplication::translate("ParameterProperties", "&New", 0));
        _listNew->setShortcut(QApplication::translate("ParameterProperties", "Alt+N", 0));
        _listUp->setText(QApplication::translate("ParameterProperties", "Move &Up", 0));
        _listUp->setShortcut(QApplication::translate("ParameterProperties", "Alt+U", 0));
        _listEdit->setText(QApplication::translate("ParameterProperties", "&Edit", 0));
        _listEdit->setShortcut(QApplication::translate("ParameterProperties", "Alt+E", 0));
        _listDown->setText(QApplication::translate("ParameterProperties", "Move &Down", 0));
        _listDown->setShortcut(QApplication::translate("ParameterProperties", "Alt+D", 0));
        _listDelete->setText(QApplication::translate("ParameterProperties", "Delete", 0));
        _active->setText(QApplication::translate("ParameterProperties", "Active", 0));
    } // retranslateUi

};

namespace Ui {
    class ParameterProperties: public Ui_ParameterProperties {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMETERPROPERTIES_H