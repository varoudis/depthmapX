/********************************************************************************
** Form generated from reading UI file 'LayerChooserDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAYERCHOOSERDLG_H
#define UI_LAYERCHOOSERDLG_H

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

class Ui_CLayerChooserDlg
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *c_text;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *c_layer_selector;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CLayerChooserDlg)
    {
        if (CLayerChooserDlg->objectName().isEmpty())
            CLayerChooserDlg->setObjectName(QString::fromUtf8("CLayerChooserDlg"));
        CLayerChooserDlg->resize(274, 134);
        verticalLayout_2 = new QVBoxLayout(CLayerChooserDlg);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        c_text = new QLabel(CLayerChooserDlg);
        c_text->setObjectName(QString::fromUtf8("c_text"));

        verticalLayout->addWidget(c_text);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CLayerChooserDlg);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        c_layer_selector = new QComboBox(CLayerChooserDlg);
        c_layer_selector->setObjectName(QString::fromUtf8("c_layer_selector"));

        horizontalLayout->addWidget(c_layer_selector);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        c_ok = new QPushButton(CLayerChooserDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout_2->addWidget(c_ok);

        c_cancel = new QPushButton(CLayerChooserDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout_2->addWidget(c_cancel);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(CLayerChooserDlg);
        QObject::connect(c_ok, SIGNAL(clicked()), CLayerChooserDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CLayerChooserDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CLayerChooserDlg);
    } // setupUi

    void retranslateUi(QDialog *CLayerChooserDlg)
    {
        CLayerChooserDlg->setWindowTitle(QApplication::translate("CLayerChooserDlg", "Choose Layer", 0, QApplication::UnicodeUTF8));
        c_text->setText(QApplication::translate("CLayerChooserDlg", "Layer\n"
"Layer", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CLayerChooserDlg", "Layer", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CLayerChooserDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CLayerChooserDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CLayerChooserDlg: public Ui_CLayerChooserDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAYERCHOOSERDLG_H
