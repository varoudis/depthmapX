/********************************************************************************
** Form generated from reading UI file 'IsovistPathDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ISOVISTPATHDLG_H
#define UI_ISOVISTPATHDLG_H

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
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CIsovistPathDlg
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QComboBox *c_fov_selection;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CIsovistPathDlg)
    {
        if (CIsovistPathDlg->objectName().isEmpty())
            CIsovistPathDlg->setObjectName(QString::fromUtf8("CIsovistPathDlg"));
        CIsovistPathDlg->resize(317, 116);
        verticalLayout_2 = new QVBoxLayout(CIsovistPathDlg);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(CIsovistPathDlg);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        c_fov_selection = new QComboBox(CIsovistPathDlg);
        c_fov_selection->setObjectName(QString::fromUtf8("c_fov_selection"));

        verticalLayout->addWidget(c_fov_selection);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        c_ok = new QPushButton(CIsovistPathDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout->addWidget(c_ok);

        c_cancel = new QPushButton(CIsovistPathDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout->addWidget(c_cancel);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(CIsovistPathDlg);
        QObject::connect(c_ok, SIGNAL(clicked()), CIsovistPathDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CIsovistPathDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CIsovistPathDlg);
    } // setupUi

    void retranslateUi(QDialog *CIsovistPathDlg)
    {
        CIsovistPathDlg->setWindowTitle(QApplication::translate("CIsovistPathDlg", "Isovist Options", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CIsovistPathDlg", "Isovist field of view", 0, QApplication::UnicodeUTF8));
        c_fov_selection->clear();
        c_fov_selection->insertItems(0, QStringList()
         << QApplication::translate("CIsovistPathDlg", "Quarter isovist (90 degrees)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CIsovistPathDlg", "Third isovist (120 degrees)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CIsovistPathDlg", "Half isovist (180 degrees)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CIsovistPathDlg", "Full isovist (360 degrees)", 0, QApplication::UnicodeUTF8)
        );
        c_ok->setText(QApplication::translate("CIsovistPathDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CIsovistPathDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CIsovistPathDlg: public Ui_CIsovistPathDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ISOVISTPATHDLG_H
