/********************************************************************************
** Form generated from reading UI file 'TopoMetDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOPOMETDLG_H
#define UI_TOPOMETDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CTopoMetDlg
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QRadioButton *c_topological;
    QRadioButton *radioButton;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *c_radius;
    QCheckBox *checkBox;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CTopoMetDlg)
    {
        if (CTopoMetDlg->objectName().isEmpty())
            CTopoMetDlg->setObjectName(QString::fromUtf8("CTopoMetDlg"));
        CTopoMetDlg->resize(311, 214);
        verticalLayout_2 = new QVBoxLayout(CTopoMetDlg);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(CTopoMetDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        c_topological = new QRadioButton(groupBox);
        c_topological->setObjectName(QString::fromUtf8("c_topological"));

        verticalLayout->addWidget(c_topological);

        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        verticalLayout->addWidget(radioButton);


        verticalLayout_2->addWidget(groupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CTopoMetDlg);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        c_radius = new QLineEdit(CTopoMetDlg);
        c_radius->setObjectName(QString::fromUtf8("c_radius"));

        horizontalLayout->addWidget(c_radius);


        verticalLayout_2->addLayout(horizontalLayout);

        checkBox = new QCheckBox(CTopoMetDlg);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        verticalLayout_2->addWidget(checkBox);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        c_ok = new QPushButton(CTopoMetDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout_2->addWidget(c_ok);

        c_cancel = new QPushButton(CTopoMetDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout_2->addWidget(c_cancel);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(CTopoMetDlg);
        QObject::connect(c_ok, SIGNAL(clicked()), CTopoMetDlg, SLOT(OnOK()));

        QMetaObject::connectSlotsByName(CTopoMetDlg);
    } // setupUi

    void retranslateUi(QDialog *CTopoMetDlg)
    {
        CTopoMetDlg->setWindowTitle(QApplication::translate("CTopoMetDlg", "Analysis Options", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CTopoMetDlg", "Analysis Type", 0, QApplication::UnicodeUTF8));
        c_topological->setText(QApplication::translate("CTopoMetDlg", "Topological (Axial)", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("CTopoMetDlg", "Metric (Physical Distance)", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CTopoMetDlg", "Radius (Metric units)", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("CTopoMetDlg", "Selected segments only\n"
"(Note: does not perform choice calculation)", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CTopoMetDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CTopoMetDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CTopoMetDlg: public Ui_CTopoMetDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOPOMETDLG_H
