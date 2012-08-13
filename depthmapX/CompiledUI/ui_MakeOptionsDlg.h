/********************************************************************************
** Form generated from reading UI file 'MakeOptionsDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAKEOPTIONSDLG_H
#define UI_MAKEOPTIONSDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CMakeOptionsDlg
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QCheckBox *c_restrict_visibility;
    QLineEdit *c_maxdist;
    QCheckBox *c_boundarygraph;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CMakeOptionsDlg)
    {
        if (CMakeOptionsDlg->objectName().isEmpty())
            CMakeOptionsDlg->setObjectName(QString::fromUtf8("CMakeOptionsDlg"));
        CMakeOptionsDlg->resize(385, 118);
        verticalLayout = new QVBoxLayout(CMakeOptionsDlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        c_restrict_visibility = new QCheckBox(CMakeOptionsDlg);
        c_restrict_visibility->setObjectName(QString::fromUtf8("c_restrict_visibility"));

        horizontalLayout->addWidget(c_restrict_visibility);

        c_maxdist = new QLineEdit(CMakeOptionsDlg);
        c_maxdist->setObjectName(QString::fromUtf8("c_maxdist"));
        c_maxdist->setEnabled(false);

        horizontalLayout->addWidget(c_maxdist);


        verticalLayout->addLayout(horizontalLayout);

        c_boundarygraph = new QCheckBox(CMakeOptionsDlg);
        c_boundarygraph->setObjectName(QString::fromUtf8("c_boundarygraph"));

        verticalLayout->addWidget(c_boundarygraph);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        c_ok = new QPushButton(CMakeOptionsDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout_2->addWidget(c_ok);

        c_cancel = new QPushButton(CMakeOptionsDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout_2->addWidget(c_cancel);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(CMakeOptionsDlg);
        QObject::connect(c_restrict_visibility, SIGNAL(clicked(bool)), CMakeOptionsDlg, SLOT(OnRestrict(bool)));
        QObject::connect(c_ok, SIGNAL(clicked()), CMakeOptionsDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CMakeOptionsDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CMakeOptionsDlg);
    } // setupUi

    void retranslateUi(QDialog *CMakeOptionsDlg)
    {
        CMakeOptionsDlg->setWindowTitle(QApplication::translate("CMakeOptionsDlg", "Make Graph Options", 0, QApplication::UnicodeUTF8));
        c_restrict_visibility->setText(QApplication::translate("CMakeOptionsDlg", "Restrict visibile distance to ", 0, QApplication::UnicodeUTF8));
        c_boundarygraph->setText(QApplication::translate("CMakeOptionsDlg", "Make boundary graph", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CMakeOptionsDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CMakeOptionsDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CMakeOptionsDlg: public Ui_CMakeOptionsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAKEOPTIONSDLG_H
