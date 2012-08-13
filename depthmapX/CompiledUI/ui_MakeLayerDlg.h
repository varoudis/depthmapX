/********************************************************************************
** Form generated from reading UI file 'MakeLayerDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAKELAYERDLG_H
#define UI_MAKELAYERDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
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

class Ui_CMakeLayerDlg
{
public:
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *c_origin;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QComboBox *c_layer_type;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QLineEdit *c_layer_name;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *c_keeporiginal;
    QCheckBox *c_push_values;
    QHBoxLayout *horizontalLayout;
    QCheckBox *c_remove_stubs;
    QLineEdit *c_percentage;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CMakeLayerDlg)
    {
        if (CMakeLayerDlg->objectName().isEmpty())
            CMakeLayerDlg->setObjectName(QString::fromUtf8("CMakeLayerDlg"));
        CMakeLayerDlg->resize(511, 318);
        verticalLayout_5 = new QVBoxLayout(CMakeLayerDlg);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(CMakeLayerDlg);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        c_origin = new QLineEdit(CMakeLayerDlg);
        c_origin->setObjectName(QString::fromUtf8("c_origin"));
        c_origin->setReadOnly(true);

        verticalLayout->addWidget(c_origin);


        verticalLayout_5->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(CMakeLayerDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        c_layer_type = new QComboBox(CMakeLayerDlg);
        c_layer_type->setObjectName(QString::fromUtf8("c_layer_type"));

        verticalLayout_2->addWidget(c_layer_type);


        verticalLayout_5->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_3 = new QLabel(CMakeLayerDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_3->addWidget(label_3);

        c_layer_name = new QLineEdit(CMakeLayerDlg);
        c_layer_name->setObjectName(QString::fromUtf8("c_layer_name"));

        verticalLayout_3->addWidget(c_layer_name);


        verticalLayout_5->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        c_keeporiginal = new QCheckBox(CMakeLayerDlg);
        c_keeporiginal->setObjectName(QString::fromUtf8("c_keeporiginal"));

        verticalLayout_4->addWidget(c_keeporiginal);

        c_push_values = new QCheckBox(CMakeLayerDlg);
        c_push_values->setObjectName(QString::fromUtf8("c_push_values"));

        verticalLayout_4->addWidget(c_push_values);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        c_remove_stubs = new QCheckBox(CMakeLayerDlg);
        c_remove_stubs->setObjectName(QString::fromUtf8("c_remove_stubs"));

        horizontalLayout->addWidget(c_remove_stubs);

        c_percentage = new QLineEdit(CMakeLayerDlg);
        c_percentage->setObjectName(QString::fromUtf8("c_percentage"));
        c_percentage->setEnabled(false);

        horizontalLayout->addWidget(c_percentage);

        label_4 = new QLabel(CMakeLayerDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout->addWidget(label_4);


        verticalLayout_4->addLayout(horizontalLayout);


        verticalLayout_5->addLayout(verticalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        c_ok = new QPushButton(CMakeLayerDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout_2->addWidget(c_ok);

        c_cancel = new QPushButton(CMakeLayerDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout_2->addWidget(c_cancel);


        verticalLayout_5->addLayout(horizontalLayout_2);


        retranslateUi(CMakeLayerDlg);
        QObject::connect(c_layer_type, SIGNAL(currentIndexChanged(int)), CMakeLayerDlg, SLOT(OnSelchangeLayerType(int)));
        QObject::connect(c_remove_stubs, SIGNAL(clicked(bool)), CMakeLayerDlg, SLOT(OnRemoveStubs(bool)));
        QObject::connect(c_ok, SIGNAL(clicked()), CMakeLayerDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CMakeLayerDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CMakeLayerDlg);
    } // setupUi

    void retranslateUi(QDialog *CMakeLayerDlg)
    {
        CMakeLayerDlg->setWindowTitle(QApplication::translate("CMakeLayerDlg", "Create New Map", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CMakeLayerDlg", "Orgin Map", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CMakeLayerDlg", "New Map Type", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CMakeLayerDlg", "New Map Name", 0, QApplication::UnicodeUTF8));
        c_keeporiginal->setText(QApplication::translate("CMakeLayerDlg", "Retain original map", 0, QApplication::UnicodeUTF8));
        c_push_values->setText(QApplication::translate("CMakeLayerDlg", "Copy attributes to new map", 0, QApplication::UnicodeUTF8));
        c_remove_stubs->setText(QApplication::translate("CMakeLayerDlg", "Remove axial stubs less than", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CMakeLayerDlg", "% of line length", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CMakeLayerDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CMakeLayerDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CMakeLayerDlg: public Ui_CMakeLayerDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAKELAYERDLG_H
