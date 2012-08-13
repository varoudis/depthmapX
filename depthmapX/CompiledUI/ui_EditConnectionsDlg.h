/********************************************************************************
** Form generated from reading UI file 'EditConnectionsDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITCONNECTIONSDLG_H
#define UI_EDITCONNECTIONSDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CEditConnectionsDlg
{
public:
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QRadioButton *c_join_type;
    QRadioButton *c_radio2;
    QRadioButton *c_radio3;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *c_pin_to_sel;
    QCheckBox *c_sel_to_pin;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CEditConnectionsDlg)
    {
        if (CEditConnectionsDlg->objectName().isEmpty())
            CEditConnectionsDlg->setObjectName(QString::fromUtf8("CEditConnectionsDlg"));
        CEditConnectionsDlg->resize(302, 256);
        verticalLayout_5 = new QVBoxLayout(CEditConnectionsDlg);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        groupBox = new QGroupBox(CEditConnectionsDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        c_join_type = new QRadioButton(groupBox);
        c_join_type->setObjectName(QString::fromUtf8("c_join_type"));

        verticalLayout->addWidget(c_join_type);

        c_radio2 = new QRadioButton(groupBox);
        c_radio2->setObjectName(QString::fromUtf8("c_radio2"));

        verticalLayout->addWidget(c_radio2);

        c_radio3 = new QRadioButton(groupBox);
        c_radio3->setObjectName(QString::fromUtf8("c_radio3"));

        verticalLayout->addWidget(c_radio3);


        verticalLayout_2->addLayout(verticalLayout);


        verticalLayout_5->addWidget(groupBox);

        groupBox_2 = new QGroupBox(CEditConnectionsDlg);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        c_pin_to_sel = new QCheckBox(groupBox_2);
        c_pin_to_sel->setObjectName(QString::fromUtf8("c_pin_to_sel"));

        verticalLayout_3->addWidget(c_pin_to_sel);

        c_sel_to_pin = new QCheckBox(groupBox_2);
        c_sel_to_pin->setObjectName(QString::fromUtf8("c_sel_to_pin"));

        verticalLayout_3->addWidget(c_sel_to_pin);


        verticalLayout_4->addLayout(verticalLayout_3);


        verticalLayout_5->addWidget(groupBox_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        c_ok = new QPushButton(CEditConnectionsDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout->addWidget(c_ok);

        c_cancel = new QPushButton(CEditConnectionsDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout->addWidget(c_cancel);


        verticalLayout_5->addLayout(horizontalLayout);


        retranslateUi(CEditConnectionsDlg);
        QObject::connect(c_ok, SIGNAL(clicked()), CEditConnectionsDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CEditConnectionsDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CEditConnectionsDlg);
    } // setupUi

    void retranslateUi(QDialog *CEditConnectionsDlg)
    {
        CEditConnectionsDlg->setWindowTitle(QApplication::translate("CEditConnectionsDlg", "Edit Connections", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CEditConnectionsDlg", "Change to make", 0, QApplication::UnicodeUTF8));
        c_join_type->setText(QApplication::translate("CEditConnectionsDlg", "&Add connections", 0, QApplication::UnicodeUTF8));
        c_radio2->setText(QApplication::translate("CEditConnectionsDlg", "&Merge connections", 0, QApplication::UnicodeUTF8));
        c_radio3->setText(QApplication::translate("CEditConnectionsDlg", "&Break connections", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("CEditConnectionsDlg", "GroupBox", 0, QApplication::UnicodeUTF8));
        c_pin_to_sel->setText(QApplication::translate("CEditConnectionsDlg", "&Selected area", 0, QApplication::UnicodeUTF8));
        c_sel_to_pin->setText(QApplication::translate("CEditConnectionsDlg", "&Pinned area", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CEditConnectionsDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CEditConnectionsDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CEditConnectionsDlg: public Ui_CEditConnectionsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITCONNECTIONSDLG_H
