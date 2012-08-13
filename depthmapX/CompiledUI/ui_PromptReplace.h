/********************************************************************************
** Form generated from reading UI file 'PromptReplace.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROMPTREPLACE_H
#define UI_PROMPTREPLACE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CPromptReplace
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *c_message;
    QHBoxLayout *horizontalLayout;
    QPushButton *c_add;
    QPushButton *c_replace;
    QPushButton *c_cancel;

    void setupUi(QDialog *CPromptReplace)
    {
        if (CPromptReplace->objectName().isEmpty())
            CPromptReplace->setObjectName(QString::fromUtf8("CPromptReplace"));
        CPromptReplace->resize(362, 112);
        verticalLayout = new QVBoxLayout(CPromptReplace);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        c_message = new QLabel(CPromptReplace);
        c_message->setObjectName(QString::fromUtf8("c_message"));

        verticalLayout->addWidget(c_message);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        c_add = new QPushButton(CPromptReplace);
        c_add->setObjectName(QString::fromUtf8("c_add"));

        horizontalLayout->addWidget(c_add);

        c_replace = new QPushButton(CPromptReplace);
        c_replace->setObjectName(QString::fromUtf8("c_replace"));

        horizontalLayout->addWidget(c_replace);

        c_cancel = new QPushButton(CPromptReplace);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout->addWidget(c_cancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(CPromptReplace);
        QObject::connect(c_add, SIGNAL(clicked()), CPromptReplace, SLOT(OnAdd()));
        QObject::connect(c_replace, SIGNAL(clicked()), CPromptReplace, SLOT(OnReplace()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CPromptReplace, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CPromptReplace);
    } // setupUi

    void retranslateUi(QDialog *CPromptReplace)
    {
        CPromptReplace->setWindowTitle(QApplication::translate("CPromptReplace", "depthmapX", 0, QApplication::UnicodeUTF8));
        c_message->setText(QApplication::translate("CPromptReplace", "You already have line data loaded.  Do you want to\n"
" add this new file to the existing line data, replace \n"
"the existing line data, or cancel?", 0, QApplication::UnicodeUTF8));
        c_add->setText(QApplication::translate("CPromptReplace", "Add", 0, QApplication::UnicodeUTF8));
        c_replace->setText(QApplication::translate("CPromptReplace", "Replace", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CPromptReplace", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CPromptReplace: public Ui_CPromptReplace {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROMPTREPLACE_H
