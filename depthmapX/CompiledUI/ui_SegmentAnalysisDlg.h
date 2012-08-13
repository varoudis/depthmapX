/********************************************************************************
** Form generated from reading UI file 'SegmentAnalysisDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEGMENTANALYSISDLG_H
#define UI_SEGMENTANALYSISDLG_H

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

class Ui_CSegmentAnalysisDlg
{
public:
    QVBoxLayout *verticalLayout_7;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *c_analysis_type;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *c_tulip_bins;
    QLabel *label_2;
    QCheckBox *c_choice;
    QRadioButton *c_radio2;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QRadioButton *c_radius_type;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButton;
    QRadioButton *c_radio3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QLineEdit *c_radius;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_6;
    QCheckBox *c_weighted;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_4;
    QComboBox *c_attribute;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CSegmentAnalysisDlg)
    {
        if (CSegmentAnalysisDlg->objectName().isEmpty())
            CSegmentAnalysisDlg->setObjectName(QString::fromUtf8("CSegmentAnalysisDlg"));
        CSegmentAnalysisDlg->resize(319, 501);
        verticalLayout_7 = new QVBoxLayout(CSegmentAnalysisDlg);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        groupBox = new QGroupBox(CSegmentAnalysisDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        c_analysis_type = new QRadioButton(groupBox);
        c_analysis_type->setObjectName(QString::fromUtf8("c_analysis_type"));

        verticalLayout_2->addWidget(c_analysis_type);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout->addWidget(label);

        c_tulip_bins = new QLineEdit(groupBox);
        c_tulip_bins->setObjectName(QString::fromUtf8("c_tulip_bins"));

        horizontalLayout->addWidget(c_tulip_bins);


        verticalLayout->addLayout(horizontalLayout);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);


        verticalLayout_2->addLayout(verticalLayout);

        c_choice = new QCheckBox(groupBox);
        c_choice->setObjectName(QString::fromUtf8("c_choice"));

        verticalLayout_2->addWidget(c_choice);

        c_radio2 = new QRadioButton(groupBox);
        c_radio2->setObjectName(QString::fromUtf8("c_radio2"));

        verticalLayout_2->addWidget(c_radio2);


        verticalLayout_7->addWidget(groupBox);

        groupBox_2 = new QGroupBox(CSegmentAnalysisDlg);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        c_radius_type = new QRadioButton(groupBox_2);
        c_radius_type->setObjectName(QString::fromUtf8("c_radius_type"));

        verticalLayout_4->addWidget(c_radius_type);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        radioButton = new QRadioButton(groupBox_2);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        horizontalLayout_2->addWidget(radioButton);

        c_radio3 = new QRadioButton(groupBox_2);
        c_radio3->setObjectName(QString::fromUtf8("c_radio3"));

        horizontalLayout_2->addWidget(c_radio3);


        verticalLayout_4->addLayout(horizontalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_3->addWidget(label_3);

        c_radius = new QLineEdit(groupBox_2);
        c_radius->setObjectName(QString::fromUtf8("c_radius"));

        verticalLayout_3->addWidget(c_radius);


        verticalLayout_4->addLayout(verticalLayout_3);


        verticalLayout_7->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(CSegmentAnalysisDlg);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_6 = new QVBoxLayout(groupBox_3);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        c_weighted = new QCheckBox(groupBox_3);
        c_weighted->setObjectName(QString::fromUtf8("c_weighted"));

        verticalLayout_6->addWidget(c_weighted);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_5->addWidget(label_4);

        c_attribute = new QComboBox(groupBox_3);
        c_attribute->setObjectName(QString::fromUtf8("c_attribute"));

        verticalLayout_5->addWidget(c_attribute);


        verticalLayout_6->addLayout(verticalLayout_5);


        verticalLayout_7->addWidget(groupBox_3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        c_ok = new QPushButton(CSegmentAnalysisDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout_3->addWidget(c_ok);

        c_cancel = new QPushButton(CSegmentAnalysisDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout_3->addWidget(c_cancel);


        verticalLayout_7->addLayout(horizontalLayout_3);


        retranslateUi(CSegmentAnalysisDlg);
        QObject::connect(c_analysis_type, SIGNAL(clicked(bool)), CSegmentAnalysisDlg, SLOT(OnAnalysisType(bool)));
        QObject::connect(c_radio2, SIGNAL(clicked(bool)), CSegmentAnalysisDlg, SLOT(OnAnalysisTulip(bool)));
        QObject::connect(c_radius, SIGNAL(textChanged(QString)), CSegmentAnalysisDlg, SLOT(OnUpdateRadius(QString)));
        QObject::connect(c_weighted, SIGNAL(clicked(bool)), CSegmentAnalysisDlg, SLOT(OnWeighted(bool)));
        QObject::connect(c_ok, SIGNAL(clicked()), CSegmentAnalysisDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CSegmentAnalysisDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CSegmentAnalysisDlg);
    } // setupUi

    void retranslateUi(QDialog *CSegmentAnalysisDlg)
    {
        CSegmentAnalysisDlg->setWindowTitle(QApplication::translate("CSegmentAnalysisDlg", "Segment Analysis Options", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CSegmentAnalysisDlg", "Analysis Type", 0, QApplication::UnicodeUTF8));
        c_analysis_type->setText(QApplication::translate("CSegmentAnalysisDlg", "Tulip Analysis (Faster)", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CSegmentAnalysisDlg", "Tulip Bins (4 to 1024)", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CSegmentAnalysisDlg", "(1024 approximates full angular analysis)", 0, QApplication::UnicodeUTF8));
        c_choice->setText(QApplication::translate("CSegmentAnalysisDlg", "Include choice (betweenness)", 0, QApplication::UnicodeUTF8));
        c_radio2->setText(QApplication::translate("CSegmentAnalysisDlg", "Full Angular (Slower)", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("CSegmentAnalysisDlg", "Radius Type", 0, QApplication::UnicodeUTF8));
        c_radius_type->setText(QApplication::translate("CSegmentAnalysisDlg", "Segment Steps", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("CSegmentAnalysisDlg", "Metric", 0, QApplication::UnicodeUTF8));
        c_radio3->setText(QApplication::translate("CSegmentAnalysisDlg", "Angular", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CSegmentAnalysisDlg", "Radius / List of radii", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("CSegmentAnalysisDlg", "Weighted measures", 0, QApplication::UnicodeUTF8));
        c_weighted->setText(QApplication::translate("CSegmentAnalysisDlg", "Include weighted measures", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CSegmentAnalysisDlg", "Weight by", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CSegmentAnalysisDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CSegmentAnalysisDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CSegmentAnalysisDlg: public Ui_CSegmentAnalysisDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEGMENTANALYSISDLG_H
