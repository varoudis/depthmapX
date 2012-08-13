/********************************************************************************
** Form generated from reading UI file 'RenameObjectDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENAMEOBJECTDLG_H
#define UI_RENAMEOBJECTDLG_H

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
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CRenameObjectDlg
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *c_prompt;
    QLineEdit *c_object_name;
    QHBoxLayout *horizontalLayout;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CRenameObjectDlg)
    {
        if (CRenameObjectDlg->objectName().isEmpty())
            CRenameObjectDlg->setObjectName(QString::fromUtf8("CRenameObjectDlg"));
        CRenameObjectDlg->resize(264, 114);
        verticalLayout_2 = new QVBoxLayout(CRenameObjectDlg);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        c_prompt = new QLabel(CRenameObjectDlg);
        c_prompt->setObjectName(QString::fromUtf8("c_prompt"));

        verticalLayout->addWidget(c_prompt);

        c_object_name = new QLineEdit(CRenameObjectDlg);
        c_object_name->setObjectName(QString::fromUtf8("c_object_name"));

        verticalLayout->addWidget(c_object_name);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        c_ok = new QPushButton(CRenameObjectDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout->addWidget(c_ok);

        c_cancel = new QPushButton(CRenameObjectDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout->addWidget(c_cancel);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(CRenameObjectDlg);
        QObject::connect(c_ok, SIGNAL(clicked()), CRenameObjectDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CRenameObjectDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CRenameObjectDlg);
    } // setupUi

    void retranslateUi(QDialog *CRenameObjectDlg)
    {
        CRenameObjectDlg->setWindowTitle(QApplication::translate("CRenameObjectDlg", "Rename Column", 0, QApplication::UnicodeUTF8));
        c_prompt->setText(QApplication::translate("CRenameObjectDlg", "Rename column to:", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CRenameObjectDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CRenameObjectDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CRenameObjectDlg: public Ui_CRenameObjectDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENAMEOBJECTDLG_H
