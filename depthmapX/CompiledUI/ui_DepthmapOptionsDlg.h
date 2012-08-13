/********************************************************************************
** Form generated from reading UI file 'DepthmapOptionsDlg.ui'
**
** Created: Thu May 3 14:21:38 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEPTHMAPOPTIONSDLG_H
#define UI_DEPTHMAPOPTIONSDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CDepthmapOptionsDlg
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QCheckBox *c_show_simple_version;
    QSpacerItem *verticalSpacer;
    QCheckBox *c_show_research_toolbar;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CDepthmapOptionsDlg)
    {
        if (CDepthmapOptionsDlg->objectName().isEmpty())
            CDepthmapOptionsDlg->setObjectName(QString::fromUtf8("CDepthmapOptionsDlg"));
        CDepthmapOptionsDlg->resize(365, 192);
        verticalLayout = new QVBoxLayout(CDepthmapOptionsDlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(CDepthmapOptionsDlg);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        c_show_simple_version = new QCheckBox(CDepthmapOptionsDlg);
        c_show_simple_version->setObjectName(QString::fromUtf8("c_show_simple_version"));
        c_show_simple_version->setChecked(true);

        verticalLayout->addWidget(c_show_simple_version);

        verticalSpacer = new QSpacerItem(20, 37, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        c_show_research_toolbar = new QCheckBox(CDepthmapOptionsDlg);
        c_show_research_toolbar->setObjectName(QString::fromUtf8("c_show_research_toolbar"));

        verticalLayout->addWidget(c_show_research_toolbar);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        c_ok = new QPushButton(CDepthmapOptionsDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout->addWidget(c_ok);

        c_cancel = new QPushButton(CDepthmapOptionsDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout->addWidget(c_cancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(CDepthmapOptionsDlg);
        QObject::connect(c_ok, SIGNAL(clicked()), CDepthmapOptionsDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CDepthmapOptionsDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CDepthmapOptionsDlg);
    } // setupUi

    void retranslateUi(QDialog *CDepthmapOptionsDlg)
    {
        CDepthmapOptionsDlg->setWindowTitle(QApplication::translate("CDepthmapOptionsDlg", "depthmapX Options", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CDepthmapOptionsDlg", "'depthmapX' runs with basic measures by default,\n"
"only change it if you know what you are doing...", 0, QApplication::UnicodeUTF8));
        c_show_simple_version->setText(QApplication::translate("CDepthmapOptionsDlg", "Simple Version", 0, QApplication::UnicodeUTF8));
        c_show_research_toolbar->setText(QApplication::translate("CDepthmapOptionsDlg", "Show research toolbar (Don't USE!)", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CDepthmapOptionsDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CDepthmapOptionsDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CDepthmapOptionsDlg: public Ui_CDepthmapOptionsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEPTHMAPOPTIONSDLG_H
