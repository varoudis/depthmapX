/********************************************************************************
** Form generated from reading UI file 'DepthmapAlert.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEPTHMAPALERT_H
#define UI_DEPTHMAPALERT_H

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
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CDepthmapAlert
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTextEdit *c_message;
    QLabel *label;
    QLabel *c_alert_link;
    QHBoxLayout *horizontalLayout;
    QCheckBox *c_read_it;
    QPushButton *c_ok;

    void setupUi(QDialog *CDepthmapAlert)
    {
        if (CDepthmapAlert->objectName().isEmpty())
            CDepthmapAlert->setObjectName(QString::fromUtf8("CDepthmapAlert"));
        CDepthmapAlert->resize(515, 263);
        verticalLayout_2 = new QVBoxLayout(CDepthmapAlert);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        c_message = new QTextEdit(CDepthmapAlert);
        c_message->setObjectName(QString::fromUtf8("c_message"));

        verticalLayout->addWidget(c_message);

        label = new QLabel(CDepthmapAlert);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        c_alert_link = new QLabel(CDepthmapAlert);
        c_alert_link->setObjectName(QString::fromUtf8("c_alert_link"));

        verticalLayout->addWidget(c_alert_link);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        c_read_it = new QCheckBox(CDepthmapAlert);
        c_read_it->setObjectName(QString::fromUtf8("c_read_it"));

        horizontalLayout->addWidget(c_read_it);

        c_ok = new QPushButton(CDepthmapAlert);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout->addWidget(c_ok);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(CDepthmapAlert);
        QObject::connect(c_ok, SIGNAL(clicked()), CDepthmapAlert, SLOT(OnOK()));

        QMetaObject::connectSlotsByName(CDepthmapAlert);
    } // setupUi

    void retranslateUi(QDialog *CDepthmapAlert)
    {
        CDepthmapAlert->setWindowTitle(QApplication::translate("CDepthmapAlert", " depthmapX Alert", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CDepthmapAlert", "To see this and other depthmapX alerts, please visit:", 0, QApplication::UnicodeUTF8));
        c_alert_link->setText(QApplication::translate("CDepthmapAlert", "(Not USED!) http://www.vr.ucl.ac.uk/depthmap/alerts", 0, QApplication::UnicodeUTF8));
        c_read_it->setText(QApplication::translate("CDepthmapAlert", "I have read this message, please do not display it again", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CDepthmapAlert", "Continue", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CDepthmapAlert: public Ui_CDepthmapAlert {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEPTHMAPALERT_H
