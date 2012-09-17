/********************************************************************************
** Form generated from reading UI file 'licenseagreement.ui'
**
** Created: Mon Sep 17 14:40:44 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LICENSEAGREEMENT_H
#define UI_LICENSEAGREEMENT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LicenseAgreement
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLabel *label_3;
    QTextBrowser *textBrowser;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *LicenseAgreement)
    {
        if (LicenseAgreement->objectName().isEmpty())
            LicenseAgreement->setObjectName(QString::fromUtf8("LicenseAgreement"));
        LicenseAgreement->resize(591, 509);
        verticalLayout_2 = new QVBoxLayout(LicenseAgreement);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(LicenseAgreement);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        label_3 = new QLabel(LicenseAgreement);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);


        verticalLayout->addLayout(horizontalLayout_2);

        textBrowser = new QTextBrowser(LicenseAgreement);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout->addWidget(textBrowser);


        verticalLayout_2->addLayout(verticalLayout);

        label = new QLabel(LicenseAgreement);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(68, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonBox = new QDialogButtonBox(LicenseAgreement);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout->addWidget(buttonBox);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(LicenseAgreement);
        QObject::connect(buttonBox, SIGNAL(accepted()), LicenseAgreement, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), LicenseAgreement, SLOT(reject()));

        QMetaObject::connectSlotsByName(LicenseAgreement);
    } // setupUi

    void retranslateUi(QDialog *LicenseAgreement)
    {
        LicenseAgreement->setWindowTitle(QApplication::translate("LicenseAgreement", "Dialog", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("LicenseAgreement", "<html><head/><body><p><img src=\":/images/depthmapX.png\"/></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("LicenseAgreement", "<html><head/><body><p><span style=\" font-size:24pt;\">depthmapX</span></p><p><span style=\" font-size:12pt;\">Multi-Platform Spatial Network Analysis Software</span></p><p><a href=\"https://github.com/SpaceGroupUCL/depthmapX\"><span style=\" text-decoration: underline; color:#0000ff;\">depthmapX on Github</span></a></p><p><span style=\" font-size:12pt;\">Copyright (c) 2012 Tasos Varoudis, UCL</span></p><p><br/></p><p><span style=\" font-size:12pt;\">fork of the original Depthmap developed by Alasdair Turner</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        textBrowser->setHtml(QApplication::translate("LicenseAgreement", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Courier New,courier'; font-size:14pt; color:#000000;\">Copywrite (c) 2011-2012, Tasos Varoudis</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Courier New,courier'; font-size:14pt; color:#000000;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Courier New,courier'; font-size:14pt; color:#0000"
                        "00;\">This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Courier New,courier'; font-size:14pt; color:#000000;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Courier New,courier'; font-size:14pt; color:#000000;\">This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:"
                        "0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Courier New,courier'; font-size:14pt; color:#000000;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Courier New,courier'; font-size:14pt; color:#000000;\">You should have received a copy of the GNU General Public License along with this program.  If not, see &lt;http://www.gnu.org/licenses/&gt;.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("LicenseAgreement", "Click \"OK\" to Accept the License or Cancel to Exit...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LicenseAgreement: public Ui_LicenseAgreement {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LICENSEAGREEMENT_H
