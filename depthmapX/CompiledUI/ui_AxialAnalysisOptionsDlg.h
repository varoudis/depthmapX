/********************************************************************************
** Form generated from reading UI file 'AxialAnalysisOptionsDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AXIALANALYSISOPTIONSDLG_H
#define UI_AXIALANALYSISOPTIONSDLG_H

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

class Ui_CAxialAnalysisOptionsDlg
{
public:
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *c_radius;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *c_choice;
    QCheckBox *c_local;
    QCheckBox *c_rra;
    QCheckBox *c_weighted;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QComboBox *c_attribute_chooser;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CAxialAnalysisOptionsDlg)
    {
        if (CAxialAnalysisOptionsDlg->objectName().isEmpty())
            CAxialAnalysisOptionsDlg->setObjectName(QString::fromUtf8("CAxialAnalysisOptionsDlg"));
        CAxialAnalysisOptionsDlg->resize(276, 296);
        CAxialAnalysisOptionsDlg->setLayoutDirection(Qt::LeftToRight);
        verticalLayout_4 = new QVBoxLayout(CAxialAnalysisOptionsDlg);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(CAxialAnalysisOptionsDlg);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        c_radius = new QLineEdit(CAxialAnalysisOptionsDlg);
        c_radius->setObjectName(QString::fromUtf8("c_radius"));

        verticalLayout->addWidget(c_radius);


        verticalLayout_4->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        c_choice = new QCheckBox(CAxialAnalysisOptionsDlg);
        c_choice->setObjectName(QString::fromUtf8("c_choice"));
        c_choice->setLayoutDirection(Qt::LeftToRight);
        c_choice->setAutoFillBackground(false);
        c_choice->setTristate(false);

        verticalLayout_2->addWidget(c_choice);

        c_local = new QCheckBox(CAxialAnalysisOptionsDlg);
        c_local->setObjectName(QString::fromUtf8("c_local"));
        c_local->setLayoutDirection(Qt::LeftToRight);

        verticalLayout_2->addWidget(c_local);

        c_rra = new QCheckBox(CAxialAnalysisOptionsDlg);
        c_rra->setObjectName(QString::fromUtf8("c_rra"));

        verticalLayout_2->addWidget(c_rra);

        c_weighted = new QCheckBox(CAxialAnalysisOptionsDlg);
        c_weighted->setObjectName(QString::fromUtf8("c_weighted"));

        verticalLayout_2->addWidget(c_weighted);


        verticalLayout_4->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_2 = new QLabel(CAxialAnalysisOptionsDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_3->addWidget(label_2);

        c_attribute_chooser = new QComboBox(CAxialAnalysisOptionsDlg);
        c_attribute_chooser->setObjectName(QString::fromUtf8("c_attribute_chooser"));

        verticalLayout_3->addWidget(c_attribute_chooser);


        verticalLayout_4->addLayout(verticalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        c_ok = new QPushButton(CAxialAnalysisOptionsDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout->addWidget(c_ok);

        c_cancel = new QPushButton(CAxialAnalysisOptionsDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout->addWidget(c_cancel);


        verticalLayout_4->addLayout(horizontalLayout);


        retranslateUi(CAxialAnalysisOptionsDlg);
        QObject::connect(c_radius, SIGNAL(textChanged(QString)), CAxialAnalysisOptionsDlg, SLOT(OnUpdateRadius()));
        QObject::connect(c_weighted, SIGNAL(clicked()), CAxialAnalysisOptionsDlg, SLOT(OnWeighted()));
        QObject::connect(c_ok, SIGNAL(clicked()), CAxialAnalysisOptionsDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CAxialAnalysisOptionsDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CAxialAnalysisOptionsDlg);
    } // setupUi

    void retranslateUi(QDialog *CAxialAnalysisOptionsDlg)
    {
        CAxialAnalysisOptionsDlg->setWindowTitle(QApplication::translate("CAxialAnalysisOptionsDlg", "Axial Analysis Options", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CAxialAnalysisOptionsDlg", "Radius / list of radii, e.g., 2,3,n", 0, QApplication::UnicodeUTF8));
        c_choice->setText(QApplication::translate("CAxialAnalysisOptionsDlg", "Include choice (betweenness)", 0, QApplication::UnicodeUTF8));
        c_local->setText(QApplication::translate("CAxialAnalysisOptionsDlg", "Include local measures", 0, QApplication::UnicodeUTF8));
        c_rra->setText(QApplication::translate("CAxialAnalysisOptionsDlg", "Include RA, RRA and total depth", 0, QApplication::UnicodeUTF8));
        c_weighted->setText(QApplication::translate("CAxialAnalysisOptionsDlg", "Include weighted measures", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CAxialAnalysisOptionsDlg", "Weight by", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CAxialAnalysisOptionsDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CAxialAnalysisOptionsDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CAxialAnalysisOptionsDlg: public Ui_CAxialAnalysisOptionsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AXIALANALYSISOPTIONSDLG_H
