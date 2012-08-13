/********************************************************************************
** Form generated from reading UI file 'FindLocDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDLOCDLG_H
#define UI_FINDLOCDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CFindLocDlg
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *c_x;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label2;
    QLineEdit *c_y;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CFindLocDlg)
    {
        if (CFindLocDlg->objectName().isEmpty())
            CFindLocDlg->setObjectName(QString::fromUtf8("CFindLocDlg"));
        CFindLocDlg->resize(207, 150);
        verticalLayout = new QVBoxLayout(CFindLocDlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CFindLocDlg);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        c_x = new QLineEdit(CFindLocDlg);
        c_x->setObjectName(QString::fromUtf8("c_x"));

        horizontalLayout->addWidget(c_x);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label2 = new QLabel(CFindLocDlg);
        label2->setObjectName(QString::fromUtf8("label2"));

        horizontalLayout_2->addWidget(label2);

        c_y = new QLineEdit(CFindLocDlg);
        c_y->setObjectName(QString::fromUtf8("c_y"));

        horizontalLayout_2->addWidget(c_y);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        c_ok = new QPushButton(CFindLocDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout_3->addWidget(c_ok);

        c_cancel = new QPushButton(CFindLocDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout_3->addWidget(c_cancel);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(CFindLocDlg);
        QObject::connect(c_ok, SIGNAL(clicked()), CFindLocDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CFindLocDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CFindLocDlg);
    } // setupUi

    void retranslateUi(QDialog *CFindLocDlg)
    {
        CFindLocDlg->setWindowTitle(QApplication::translate("CFindLocDlg", "Find Location", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CFindLocDlg", "x", 0, QApplication::UnicodeUTF8));
        label2->setText(QApplication::translate("CFindLocDlg", "y", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CFindLocDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CFindLocDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CFindLocDlg: public Ui_CFindLocDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDLOCDLG_H
