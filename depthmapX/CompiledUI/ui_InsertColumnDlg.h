/********************************************************************************
** Form generated from reading UI file 'InsertColumnDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INSERTCOLUMNDLG_H
#define UI_INSERTCOLUMNDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CInsertColumnDlg
{
public:
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *c_formula_desc;
    QTextEdit *c_formula;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QListWidget *c_column_names;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *c_selection_desc;
    QPushButton *c_use_column;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CInsertColumnDlg)
    {
        if (CInsertColumnDlg->objectName().isEmpty())
            CInsertColumnDlg->setObjectName(QString::fromUtf8("CInsertColumnDlg"));
        CInsertColumnDlg->resize(546, 296);
        verticalLayout_3 = new QVBoxLayout(CInsertColumnDlg);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        c_formula_desc = new QLabel(CInsertColumnDlg);
        c_formula_desc->setObjectName(QString::fromUtf8("c_formula_desc"));

        verticalLayout->addWidget(c_formula_desc);

        c_formula = new QTextEdit(CInsertColumnDlg);
        c_formula->setObjectName(QString::fromUtf8("c_formula"));

        verticalLayout->addWidget(c_formula);


        horizontalLayout_3->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(CInsertColumnDlg);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        c_column_names = new QListWidget(CInsertColumnDlg);
        c_column_names->setObjectName(QString::fromUtf8("c_column_names"));

        verticalLayout_2->addWidget(c_column_names);


        horizontalLayout_3->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        c_selection_desc = new QCheckBox(CInsertColumnDlg);
        c_selection_desc->setObjectName(QString::fromUtf8("c_selection_desc"));

        horizontalLayout_2->addWidget(c_selection_desc);

        c_use_column = new QPushButton(CInsertColumnDlg);
        c_use_column->setObjectName(QString::fromUtf8("c_use_column"));

        horizontalLayout_2->addWidget(c_use_column);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        c_ok = new QPushButton(CInsertColumnDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout->addWidget(c_ok);

        c_cancel = new QPushButton(CInsertColumnDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout->addWidget(c_cancel);


        verticalLayout_3->addLayout(horizontalLayout);


        retranslateUi(CInsertColumnDlg);
        QObject::connect(c_use_column, SIGNAL(clicked()), CInsertColumnDlg, SLOT(OnUseAttribute()));
        QObject::connect(c_ok, SIGNAL(clicked()), CInsertColumnDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CInsertColumnDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CInsertColumnDlg);
    } // setupUi

    void retranslateUi(QDialog *CInsertColumnDlg)
    {
        CInsertColumnDlg->setWindowTitle(QApplication::translate("CInsertColumnDlg", "Replace Attribute Values", 0, QApplication::UnicodeUTF8));
        c_formula_desc->setText(QApplication::translate("CInsertColumnDlg", "Formula", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CInsertColumnDlg", "Existing attributes", 0, QApplication::UnicodeUTF8));
        c_selection_desc->setText(QApplication::translate("CInsertColumnDlg", "Apply formula to selected objects only", 0, QApplication::UnicodeUTF8));
        c_use_column->setText(QApplication::translate("CInsertColumnDlg", "<< Use attribute", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CInsertColumnDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CInsertColumnDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CInsertColumnDlg: public Ui_CInsertColumnDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INSERTCOLUMNDLG_H
