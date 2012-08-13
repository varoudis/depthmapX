/********************************************************************************
** Form generated from reading UI file 'AgentAnalysisDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AGENTANALYSISDLG_H
#define UI_AGENTANALYSISDLG_H

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
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CAgentAnalysisDlg
{
public:
    QGroupBox *groupBox;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *c_timesteps;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *c_layer_selector;
    QGroupBox *groupBox_2;
    QRadioButton *c_release_location;
    QRadioButton *c_radio2;
    QWidget *widget2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *c_release_rate;
    QGroupBox *groupBox_3;
    QWidget *widget3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *c_fov;
    QWidget *widget4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *c_steps;
    QWidget *widget5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QLineEdit *c_frames;
    QWidget *widget6;
    QHBoxLayout *horizontalLayout_7;
    QCheckBox *c_record_trails;
    QLineEdit *c_trail_count;
    QLabel *label_7;
    QWidget *widget7;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_8;
    QComboBox *c_occlusion;
    QWidget *widget8;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CAgentAnalysisDlg)
    {
        if (CAgentAnalysisDlg->objectName().isEmpty())
            CAgentAnalysisDlg->setObjectName(QString::fromUtf8("CAgentAnalysisDlg"));
        CAgentAnalysisDlg->resize(380, 557);
        groupBox = new QGroupBox(CAgentAnalysisDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 361, 111));
        widget = new QWidget(groupBox);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(20, 30, 320, 24));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        c_timesteps = new QLineEdit(widget);
        c_timesteps->setObjectName(QString::fromUtf8("c_timesteps"));

        horizontalLayout->addWidget(c_timesteps);

        widget1 = new QWidget(groupBox);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(20, 70, 321, 26));
        horizontalLayout_2 = new QHBoxLayout(widget1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget1);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        c_layer_selector = new QComboBox(widget1);
        c_layer_selector->setObjectName(QString::fromUtf8("c_layer_selector"));
        c_layer_selector->setFrame(true);
        c_layer_selector->setModelColumn(0);

        horizontalLayout_2->addWidget(c_layer_selector);

        groupBox_2 = new QGroupBox(CAgentAnalysisDlg);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 130, 361, 131));
        c_release_location = new QRadioButton(groupBox_2);
        c_release_location->setObjectName(QString::fromUtf8("c_release_location"));
        c_release_location->setGeometry(QRect(20, 60, 191, 21));
        c_radio2 = new QRadioButton(groupBox_2);
        c_radio2->setObjectName(QString::fromUtf8("c_radio2"));
        c_radio2->setGeometry(QRect(20, 90, 231, 21));
        widget2 = new QWidget(groupBox_2);
        widget2->setObjectName(QString::fromUtf8("widget2"));
        widget2->setGeometry(QRect(20, 30, 321, 24));
        horizontalLayout_3 = new QHBoxLayout(widget2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(widget2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        c_release_rate = new QLineEdit(widget2);
        c_release_rate->setObjectName(QString::fromUtf8("c_release_rate"));

        horizontalLayout_3->addWidget(c_release_rate);

        groupBox_3 = new QGroupBox(CAgentAnalysisDlg);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 270, 361, 141));
        widget3 = new QWidget(groupBox_3);
        widget3->setObjectName(QString::fromUtf8("widget3"));
        widget3->setGeometry(QRect(21, 31, 321, 24));
        horizontalLayout_4 = new QHBoxLayout(widget3);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(widget3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        c_fov = new QLineEdit(widget3);
        c_fov->setObjectName(QString::fromUtf8("c_fov"));

        horizontalLayout_4->addWidget(c_fov);

        widget4 = new QWidget(groupBox_3);
        widget4->setObjectName(QString::fromUtf8("widget4"));
        widget4->setGeometry(QRect(20, 70, 320, 24));
        horizontalLayout_5 = new QHBoxLayout(widget4);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(widget4);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_5->addWidget(label_5);

        c_steps = new QLineEdit(widget4);
        c_steps->setObjectName(QString::fromUtf8("c_steps"));

        horizontalLayout_5->addWidget(c_steps);

        widget5 = new QWidget(groupBox_3);
        widget5->setObjectName(QString::fromUtf8("widget5"));
        widget5->setGeometry(QRect(20, 110, 321, 24));
        horizontalLayout_6 = new QHBoxLayout(widget5);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(widget5);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_6->addWidget(label_6);

        c_frames = new QLineEdit(widget5);
        c_frames->setObjectName(QString::fromUtf8("c_frames"));

        horizontalLayout_6->addWidget(c_frames);

        widget6 = new QWidget(CAgentAnalysisDlg);
        widget6->setObjectName(QString::fromUtf8("widget6"));
        widget6->setGeometry(QRect(30, 430, 321, 29));
        horizontalLayout_7 = new QHBoxLayout(widget6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        c_record_trails = new QCheckBox(widget6);
        c_record_trails->setObjectName(QString::fromUtf8("c_record_trails"));

        horizontalLayout_7->addWidget(c_record_trails);

        c_trail_count = new QLineEdit(widget6);
        c_trail_count->setObjectName(QString::fromUtf8("c_trail_count"));

        horizontalLayout_7->addWidget(c_trail_count);

        label_7 = new QLabel(widget6);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_7->addWidget(label_7);

        widget7 = new QWidget(CAgentAnalysisDlg);
        widget7->setObjectName(QString::fromUtf8("widget7"));
        widget7->setGeometry(QRect(30, 470, 321, 26));
        horizontalLayout_8 = new QHBoxLayout(widget7);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        label_8 = new QLabel(widget7);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_8->addWidget(label_8);

        c_occlusion = new QComboBox(widget7);
        c_occlusion->setObjectName(QString::fromUtf8("c_occlusion"));

        horizontalLayout_8->addWidget(c_occlusion);

        widget8 = new QWidget(CAgentAnalysisDlg);
        widget8->setObjectName(QString::fromUtf8("widget8"));
        widget8->setGeometry(QRect(190, 510, 164, 32));
        horizontalLayout_9 = new QHBoxLayout(widget8);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        c_ok = new QPushButton(widget8);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout_9->addWidget(c_ok);

        c_cancel = new QPushButton(widget8);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout_9->addWidget(c_cancel);


        retranslateUi(CAgentAnalysisDlg);
        QObject::connect(c_ok, SIGNAL(clicked()), CAgentAnalysisDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CAgentAnalysisDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CAgentAnalysisDlg);
    } // setupUi

    void retranslateUi(QDialog *CAgentAnalysisDlg)
    {
        CAgentAnalysisDlg->setWindowTitle(QApplication::translate("CAgentAnalysisDlg", "Agent Analysis Setup", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CAgentAnalysisDlg", "Global setup", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CAgentAnalysisDlg", "Analysis length(timesteps)", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CAgentAnalysisDlg", "Record gate counts in data map", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("CAgentAnalysisDlg", "Agent set parameters", 0, QApplication::UnicodeUTF8));
        c_release_location->setText(QApplication::translate("CAgentAnalysisDlg", "Release from any location", 0, QApplication::UnicodeUTF8));
        c_radio2->setText(QApplication::translate("CAgentAnalysisDlg", "Release from selected locations", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CAgentAnalysisDlg", "Release rate (agents per timestep)", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("CAgentAnalysisDlg", "Agent program parameters", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CAgentAnalysisDlg", "Field of view (bins)", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CAgentAnalysisDlg", "Steps before turn decision", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CAgentAnalysisDlg", "Timesteps in system", 0, QApplication::UnicodeUTF8));
        c_record_trails->setText(QApplication::translate("CAgentAnalysisDlg", "Record trails for", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("CAgentAnalysisDlg", "agents", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("CAgentAnalysisDlg", "Movement rule:", 0, QApplication::UnicodeUTF8));
        c_occlusion->clear();
        c_occlusion->insertItems(0, QStringList()
         << QApplication::translate("CAgentAnalysisDlg", "Standard", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CAgentAnalysisDlg", "Line of Sight Length", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CAgentAnalysisDlg", "Occluded Length", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CAgentAnalysisDlg", "Any occlusions", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CAgentAnalysisDlg", "Occlusions Group bins (45 degrees)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CAgentAnalysisDlg", "Occlusions Group bins (60 degrees)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CAgentAnalysisDlg", "Furthest occlusion per bin", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CAgentAnalysisDlg", "Per bin far distance weighted", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CAgentAnalysisDlg", "Per bin angle weighted", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CAgentAnalysisDlg", "Per bin far distance and angle weighted", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CAgentAnalysisDlg", "Per bin memory", 0, QApplication::UnicodeUTF8)
        );
        c_ok->setText(QApplication::translate("CAgentAnalysisDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CAgentAnalysisDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CAgentAnalysisDlg: public Ui_CAgentAnalysisDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AGENTANALYSISDLG_H
