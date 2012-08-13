/********************************************************************************
** Form generated from reading UI file 'AttributeSummary.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ATTRIBUTESUMMARY_H
#define UI_ATTRIBUTESUMMARY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CAttributeSummary
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *c_list;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;

    void setupUi(QDialog *CAttributeSummary)
    {
        if (CAttributeSummary->objectName().isEmpty())
            CAttributeSummary->setObjectName(QString::fromUtf8("CAttributeSummary"));
        CAttributeSummary->resize(531, 319);
        verticalLayout = new QVBoxLayout(CAttributeSummary);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        c_list = new QTableWidget(CAttributeSummary);
        c_list->setObjectName(QString::fromUtf8("c_list"));

        verticalLayout->addWidget(c_list);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(388, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        c_ok = new QPushButton(CAttributeSummary);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout->addWidget(c_ok);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(CAttributeSummary);
        QObject::connect(c_ok, SIGNAL(clicked()), CAttributeSummary, SLOT(OnOK()));

        QMetaObject::connectSlotsByName(CAttributeSummary);
    } // setupUi

    void retranslateUi(QDialog *CAttributeSummary)
    {
        CAttributeSummary->setWindowTitle(QApplication::translate("CAttributeSummary", "Attribute Summary", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CAttributeSummary", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CAttributeSummary: public Ui_CAttributeSummary {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ATTRIBUTESUMMARY_H
