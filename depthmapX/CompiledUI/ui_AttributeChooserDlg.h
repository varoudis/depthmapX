/********************************************************************************
** Form generated from reading UI file 'AttributeChooserDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ATTRIBUTECHOOSERDLG_H
#define UI_ATTRIBUTECHOOSERDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CAttributeChooserDlg
{
public:
    QLabel *c_text;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *c_attribute_chooser;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CAttributeChooserDlg)
    {
        if (CAttributeChooserDlg->objectName().isEmpty())
            CAttributeChooserDlg->setObjectName(QString::fromUtf8("CAttributeChooserDlg"));
        CAttributeChooserDlg->resize(342, 152);
        c_text = new QLabel(CAttributeChooserDlg);
        c_text->setObjectName(QString::fromUtf8("c_text"));
        c_text->setGeometry(QRect(20, 20, 311, 51));
        widget = new QWidget(CAttributeChooserDlg);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(20, 80, 301, 26));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        c_attribute_chooser = new QComboBox(widget);
        c_attribute_chooser->setObjectName(QString::fromUtf8("c_attribute_chooser"));

        horizontalLayout->addWidget(c_attribute_chooser);

        widget1 = new QWidget(CAttributeChooserDlg);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(160, 110, 164, 32));
        horizontalLayout_2 = new QHBoxLayout(widget1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        c_ok = new QPushButton(widget1);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout_2->addWidget(c_ok);

        c_cancel = new QPushButton(widget1);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout_2->addWidget(c_cancel);


        retranslateUi(CAttributeChooserDlg);
        QObject::connect(c_ok, SIGNAL(clicked()), CAttributeChooserDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CAttributeChooserDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CAttributeChooserDlg);
    } // setupUi

    void retranslateUi(QDialog *CAttributeChooserDlg)
    {
        CAttributeChooserDlg->setWindowTitle(QApplication::translate("CAttributeChooserDlg", "Choose Attribute", 0, QApplication::UnicodeUTF8));
        c_text->setText(QApplication::translate("CAttributeChooserDlg", "Attribute\n"
"Atrribute", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CAttributeChooserDlg", "Attribute", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CAttributeChooserDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CAttributeChooserDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CAttributeChooserDlg: public Ui_CAttributeChooserDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ATTRIBUTECHOOSERDLG_H
