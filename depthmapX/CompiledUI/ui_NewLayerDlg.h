/********************************************************************************
** Form generated from reading UI file 'NewLayerDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWLAYERDLG_H
#define UI_NEWLAYERDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CNewLayerDlg
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *c_layer_selector;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *c_name;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CNewLayerDlg)
    {
        if (CNewLayerDlg->objectName().isEmpty())
            CNewLayerDlg->setObjectName(QString::fromUtf8("CNewLayerDlg"));
        CNewLayerDlg->resize(255, 126);
        verticalLayout = new QVBoxLayout(CNewLayerDlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CNewLayerDlg);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        c_layer_selector = new QComboBox(CNewLayerDlg);
        c_layer_selector->setObjectName(QString::fromUtf8("c_layer_selector"));

        horizontalLayout->addWidget(c_layer_selector);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(CNewLayerDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        c_name = new QLineEdit(CNewLayerDlg);
        c_name->setObjectName(QString::fromUtf8("c_name"));

        horizontalLayout_2->addWidget(c_name);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        c_ok = new QPushButton(CNewLayerDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout_3->addWidget(c_ok);

        c_cancel = new QPushButton(CNewLayerDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout_3->addWidget(c_cancel);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(CNewLayerDlg);
        QObject::connect(c_layer_selector, SIGNAL(currentIndexChanged(int)), CNewLayerDlg, SLOT(OnSelchangeLayerType(int)));
        QObject::connect(c_ok, SIGNAL(clicked()), CNewLayerDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CNewLayerDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CNewLayerDlg);
    } // setupUi

    void retranslateUi(QDialog *CNewLayerDlg)
    {
        CNewLayerDlg->setWindowTitle(QApplication::translate("CNewLayerDlg", "New Map", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CNewLayerDlg", "Map type", 0, QApplication::UnicodeUTF8));
        c_layer_selector->clear();
        c_layer_selector->insertItems(0, QStringList()
         << QApplication::translate("CNewLayerDlg", "Data Map", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CNewLayerDlg", "Convex Map", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CNewLayerDlg", "Axial Map", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CNewLayerDlg", "Pesh Map", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("CNewLayerDlg", "Name", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CNewLayerDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CNewLayerDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CNewLayerDlg: public Ui_CNewLayerDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWLAYERDLG_H
