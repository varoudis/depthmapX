/********************************************************************************
** Form generated from reading UI file 'OptionsDlg.ui'
**
** Created: Fri Apr 27 10:29:55 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONSDLG_H
#define UI_OPTIONSDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
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

class Ui_COptionsDlg
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QRadioButton *c_output_type;
    QRadioButton *c_radio1;
    QHBoxLayout *horizontalLayout;
    QCheckBox *c_global;
    QLineEdit *c_radius;
    QCheckBox *c_local;
    QRadioButton *c_radio2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *c_radius2;
    QRadioButton *c_radio3;
    QRadioButton *c_radio4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QComboBox *c_layer_selector;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *COptionsDlg)
    {
        if (COptionsDlg->objectName().isEmpty())
            COptionsDlg->setObjectName(QString::fromUtf8("COptionsDlg"));
        COptionsDlg->resize(577, 338);
        verticalLayout_2 = new QVBoxLayout(COptionsDlg);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox = new QGroupBox(COptionsDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        c_output_type = new QRadioButton(groupBox);
        c_output_type->setObjectName(QString::fromUtf8("c_output_type"));

        verticalLayout->addWidget(c_output_type);

        c_radio1 = new QRadioButton(groupBox);
        c_radio1->setObjectName(QString::fromUtf8("c_radio1"));

        verticalLayout->addWidget(c_radio1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        c_global = new QCheckBox(groupBox);
        c_global->setObjectName(QString::fromUtf8("c_global"));

        horizontalLayout->addWidget(c_global);

        c_radius = new QLineEdit(groupBox);
        c_radius->setObjectName(QString::fromUtf8("c_radius"));

        horizontalLayout->addWidget(c_radius);


        verticalLayout->addLayout(horizontalLayout);

        c_local = new QCheckBox(groupBox);
        c_local->setObjectName(QString::fromUtf8("c_local"));

        verticalLayout->addWidget(c_local);

        c_radio2 = new QRadioButton(groupBox);
        c_radio2->setObjectName(QString::fromUtf8("c_radio2"));

        verticalLayout->addWidget(c_radio2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        c_radius2 = new QLineEdit(groupBox);
        c_radius2->setObjectName(QString::fromUtf8("c_radius2"));

        horizontalLayout_2->addWidget(c_radius2);


        verticalLayout->addLayout(horizontalLayout_2);

        c_radio3 = new QRadioButton(groupBox);
        c_radio3->setObjectName(QString::fromUtf8("c_radio3"));

        verticalLayout->addWidget(c_radio3);

        c_radio4 = new QRadioButton(groupBox);
        c_radio4->setObjectName(QString::fromUtf8("c_radio4"));

        verticalLayout->addWidget(c_radio4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        c_layer_selector = new QComboBox(groupBox);
        c_layer_selector->setObjectName(QString::fromUtf8("c_layer_selector"));

        horizontalLayout_3->addWidget(c_layer_selector);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addWidget(groupBox);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        c_ok = new QPushButton(COptionsDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout_4->addWidget(c_ok);

        c_cancel = new QPushButton(COptionsDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout_4->addWidget(c_cancel);


        verticalLayout_2->addLayout(horizontalLayout_4);


        retranslateUi(COptionsDlg);
        QObject::connect(c_output_type, SIGNAL(clicked(bool)), COptionsDlg, SLOT(OnOutputType(bool)));
        QObject::connect(c_radio1, SIGNAL(clicked(bool)), COptionsDlg, SLOT(OnOutputType(bool)));
        QObject::connect(c_radio2, SIGNAL(clicked(bool)), COptionsDlg, SLOT(OnOutputType(bool)));
        QObject::connect(c_radio3, SIGNAL(clicked(bool)), COptionsDlg, SLOT(OnOutputType(bool)));
        QObject::connect(c_radio4, SIGNAL(clicked(bool)), COptionsDlg, SLOT(OnOutputType(bool)));
        QObject::connect(c_radius, SIGNAL(textChanged(QString)), COptionsDlg, SLOT(OnUpdateRadius(QString)));
        QObject::connect(c_radius2, SIGNAL(textChanged(QString)), COptionsDlg, SLOT(OnUpdateRadius2(QString)));
        QObject::connect(c_ok, SIGNAL(clicked()), COptionsDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), COptionsDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(COptionsDlg);
    } // setupUi

    void retranslateUi(QDialog *COptionsDlg)
    {
        COptionsDlg->setWindowTitle(QApplication::translate("COptionsDlg", "Analysis Options", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("COptionsDlg", "Analysis Type", 0, QApplication::UnicodeUTF8));
        c_output_type->setText(QApplication::translate("COptionsDlg", "Calculate isovist properties", 0, QApplication::UnicodeUTF8));
        c_radio1->setText(QApplication::translate("COptionsDlg", "Calculate visibility relationships", 0, QApplication::UnicodeUTF8));
        c_global->setText(QApplication::translate("COptionsDlg", "Include &global measures |    Select radius (n or number)->", 0, QApplication::UnicodeUTF8));
        c_local->setText(QApplication::translate("COptionsDlg", "Include &local measures", 0, QApplication::UnicodeUTF8));
        c_radio2->setText(QApplication::translate("COptionsDlg", "Calculate metric relationships", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("COptionsDlg", "Radius", 0, QApplication::UnicodeUTF8));
        c_radio3->setText(QApplication::translate("COptionsDlg", "Calculate angular relationships", 0, QApplication::UnicodeUTF8));
        c_radio4->setText(QApplication::translate("COptionsDlg", "Calculate through vision", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("COptionsDlg", "Record gate counts in data map", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("COptionsDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("COptionsDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class COptionsDlg: public Ui_COptionsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONSDLG_H
