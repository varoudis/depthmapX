/********************************************************************************
** Form generated from reading UI file 'GridDialog.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIDDIALOG_H
#define UI_GRIDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CGridDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDoubleSpinBox *c_spacing_ctrl;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CGridDialog)
    {
        if (CGridDialog->objectName().isEmpty())
            CGridDialog->setObjectName(QString::fromUtf8("CGridDialog"));
        CGridDialog->resize(248, 93);
        verticalLayout = new QVBoxLayout(CGridDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CGridDialog);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        c_spacing_ctrl = new QDoubleSpinBox(CGridDialog);
        c_spacing_ctrl->setObjectName(QString::fromUtf8("c_spacing_ctrl"));
        c_spacing_ctrl->setDecimals(3);
        c_spacing_ctrl->setSingleStep(0.001);

        horizontalLayout->addWidget(c_spacing_ctrl);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        c_ok = new QPushButton(CGridDialog);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout_2->addWidget(c_ok);

        c_cancel = new QPushButton(CGridDialog);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout_2->addWidget(c_cancel);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(CGridDialog);
        QObject::connect(c_spacing_ctrl, SIGNAL(valueChanged(double)), CGridDialog, SLOT(OnDeltaposSpinSpacing(double)));
        QObject::connect(c_ok, SIGNAL(clicked()), CGridDialog, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CGridDialog, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CGridDialog);
    } // setupUi

    void retranslateUi(QDialog *CGridDialog)
    {
        CGridDialog->setWindowTitle(QApplication::translate("CGridDialog", "Set Grid Properties", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CGridDialog", "Spacing", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CGridDialog", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CGridDialog", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CGridDialog: public Ui_CGridDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIDDIALOG_H
