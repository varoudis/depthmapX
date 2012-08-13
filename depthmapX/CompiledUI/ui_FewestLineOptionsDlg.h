/********************************************************************************
** Form generated from reading UI file 'FewestLineOptionsDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FEWESTLINEOPTIONSDLG_H
#define UI_FEWESTLINEOPTIONSDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CFewestLineOptionsDlg
{
public:
    QVBoxLayout *verticalLayout;
    QRadioButton *c_option;
    QRadioButton *c_radio2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CFewestLineOptionsDlg)
    {
        if (CFewestLineOptionsDlg->objectName().isEmpty())
            CFewestLineOptionsDlg->setObjectName(QString::fromUtf8("CFewestLineOptionsDlg"));
        CFewestLineOptionsDlg->resize(261, 99);
        verticalLayout = new QVBoxLayout(CFewestLineOptionsDlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        c_option = new QRadioButton(CFewestLineOptionsDlg);
        c_option->setObjectName(QString::fromUtf8("c_option"));

        verticalLayout->addWidget(c_option);

        c_radio2 = new QRadioButton(CFewestLineOptionsDlg);
        c_radio2->setObjectName(QString::fromUtf8("c_radio2"));

        verticalLayout->addWidget(c_radio2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        c_ok = new QPushButton(CFewestLineOptionsDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout->addWidget(c_ok);

        c_cancel = new QPushButton(CFewestLineOptionsDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout->addWidget(c_cancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(CFewestLineOptionsDlg);
        QObject::connect(c_ok, SIGNAL(clicked()), CFewestLineOptionsDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CFewestLineOptionsDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CFewestLineOptionsDlg);
    } // setupUi

    void retranslateUi(QDialog *CFewestLineOptionsDlg)
    {
        CFewestLineOptionsDlg->setWindowTitle(QApplication::translate("CFewestLineOptionsDlg", "Make Fewest Line Map Options", 0, QApplication::UnicodeUTF8));
        c_option->setText(QApplication::translate("CFewestLineOptionsDlg", "Remove all subsets", 0, QApplication::UnicodeUTF8));
        c_radio2->setText(QApplication::translate("CFewestLineOptionsDlg", "Reduce to fewest connections", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CFewestLineOptionsDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CFewestLineOptionsDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CFewestLineOptionsDlg: public Ui_CFewestLineOptionsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FEWESTLINEOPTIONSDLG_H
