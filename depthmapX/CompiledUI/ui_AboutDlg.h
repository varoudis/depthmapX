/********************************************************************************
** Form generated from reading UI file 'AboutDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDLG_H
#define UI_ABOUTDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CAboutDlg
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *c_version_info;
    QLabel *c_copyright;
    QLabel *label_3;
    QLabel *label_5;
    QTextEdit *c_agreement;
    QLabel *label_4;
    QLabel *c_vrlink;
    QPushButton *c_ok;

    void setupUi(QDialog *CAboutDlg)
    {
        if (CAboutDlg->objectName().isEmpty())
            CAboutDlg->setObjectName(QString::fromUtf8("CAboutDlg"));
        CAboutDlg->resize(499, 420);
        layoutWidget = new QWidget(CAboutDlg);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 20, 461, 381));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        c_version_info = new QLabel(layoutWidget);
        c_version_info->setObjectName(QString::fromUtf8("c_version_info"));

        verticalLayout->addWidget(c_version_info);

        c_copyright = new QLabel(layoutWidget);
        c_copyright->setObjectName(QString::fromUtf8("c_copyright"));

        verticalLayout->addWidget(c_copyright);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout->addWidget(label_5);

        c_agreement = new QTextEdit(layoutWidget);
        c_agreement->setObjectName(QString::fromUtf8("c_agreement"));
        c_agreement->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        c_agreement->setReadOnly(true);

        verticalLayout->addWidget(c_agreement);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout->addWidget(label_4);

        c_vrlink = new QLabel(layoutWidget);
        c_vrlink->setObjectName(QString::fromUtf8("c_vrlink"));

        verticalLayout->addWidget(c_vrlink);

        c_ok = new QPushButton(layoutWidget);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        verticalLayout->addWidget(c_ok);


        retranslateUi(CAboutDlg);
        QObject::connect(c_ok, SIGNAL(clicked()), CAboutDlg, SLOT(OnOK()));

        QMetaObject::connectSlotsByName(CAboutDlg);
    } // setupUi

    void retranslateUi(QDialog *CAboutDlg)
    {
        CAboutDlg->setWindowTitle(QApplication::translate("CAboutDlg", "About depthmapX", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CAboutDlg", "depthmapX", 0, QApplication::UnicodeUTF8));
        c_version_info->setText(QApplication::translate("CAboutDlg", "Version information", 0, QApplication::UnicodeUTF8));
        c_copyright->setText(QApplication::translate("CAboutDlg", "Copyright message (overwritten at runtime)", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CAboutDlg", "Original Depthmap developed by Alasdair Turner.", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CAboutDlg", "<html><head/><body><p><a href=\"http://en.wikipedia.org/wiki/Alasdair_Turner\"><span style=\" text-decoration: underline; color:#0000ff;\">http://en.wikipedia.org/wiki/Alasdair_Turner</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CAboutDlg", "For more information or to submit a bug see:", 0, QApplication::UnicodeUTF8));
        c_vrlink->setText(QApplication::translate("CAboutDlg", "<html><head/><body><p><a href=\"https://github.com/SpaceGroupUCL/depthmapX\"><span style=\" text-decoration: underline; color:#0000ff;\">https://github.com/SpaceGroupUCL/depthmapX</span></a></p></body></html>", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CAboutDlg", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CAboutDlg: public Ui_CAboutDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDLG_H
