/********************************************************************************
** Form generated from reading UI file 'LicenceDialog.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LICENCEDIALOG_H
#define UI_LICENCEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CLicenceDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *c_message;
    QSpacerItem *horizontalSpacer;
    QTextEdit *c_agreement;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CLicenceDialog)
    {
        if (CLicenceDialog->objectName().isEmpty())
            CLicenceDialog->setObjectName(QString::fromUtf8("CLicenceDialog"));
        CLicenceDialog->resize(364, 274);
        verticalLayout_2 = new QVBoxLayout(CLicenceDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(CLicenceDialog);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        c_message = new QLabel(CLicenceDialog);
        c_message->setObjectName(QString::fromUtf8("c_message"));
        c_message->setTextFormat(Qt::AutoText);

        horizontalLayout->addWidget(c_message);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        c_agreement = new QTextEdit(CLicenceDialog);
        c_agreement->setObjectName(QString::fromUtf8("c_agreement"));
        c_agreement->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        c_agreement->setReadOnly(true);

        verticalLayout->addWidget(c_agreement);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        c_ok = new QPushButton(CLicenceDialog);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout_2->addWidget(c_ok);

        c_cancel = new QPushButton(CLicenceDialog);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout_2->addWidget(c_cancel);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(CLicenceDialog);
        QObject::connect(c_ok, SIGNAL(clicked()), CLicenceDialog, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CLicenceDialog, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CLicenceDialog);
    } // setupUi

    void retranslateUi(QDialog *CLicenceDialog)
    {
        CLicenceDialog->setWindowTitle(QApplication::translate("CLicenceDialog", "depthmapX", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CLicenceDialog", "Welcome to depthmapX", 0, QApplication::UnicodeUTF8));
        c_message->setText(QApplication::translate("CLicenceDialog", "Licence message", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CLicenceDialog", "Accept", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CLicenceDialog", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CLicenceDialog: public Ui_CLicenceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LICENCEDIALOG_H
