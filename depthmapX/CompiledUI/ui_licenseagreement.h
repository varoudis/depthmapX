/********************************************************************************
** Form generated from reading UI file 'licenseagreement.ui'
**
** Created: Tue May 15 16:15:14 2012
**      by: Qt User Interface Compiler version 4.8.1
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
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; color:#000000;\">\342\200\230depthmapX\342\200\231 Software Evaluation Agreement</span><span style=\" font-size:10pt;\"> </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">You (hereafter referred to as the \342\200\230Licensee\342\200\231) wishes to evaluate the Software and the Documentation developed by Tasos Varoudis, UCL (defined below and hereafter referred to as the \342\200\230Licensor\342\200\231) "
                        "and by using the Software, you confirm that you are willing to evaluate the Software at your own risk subject to the terms and conditions of this Software Evaluation Agreement. </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">1</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\">\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-size:10pt;\">Definitions </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">\342\200\230Documentation\342\200\231 shall mean any literature or data supplied with the Software by Tasos Varoudis. </"
                        "span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">\342\200\230Evaluation Period\342\200\231 shall mean the period of time, commencing on the date when this agreement came into force and ending 90 days later. </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">\342\200\230Software\342\200\231 shall mean the depthmapX software to be licensed under this Agreement. </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">2</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\""
                        ">\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-size:10pt;\">Software licence </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">2.1</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\">\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-size:10pt;\">Licensor hereby grants Licensee the non-exclusive right to use the Software for the purpose of internal evaluation only during the Evaluation Period at the Site. </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">2.2</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\">\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-size:10pt;\">Licensee agrees and undertakes to use the Software and"
                        " to undertake its evaluation without charge to Licensor for the Evaluation Period. </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">2.3</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\">\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-size:10pt;\">Licensee may at any time during the Evaluation Period, and must at the end of the Evaluation Period if Licensee decides not to enter into a further agreement with Licensor, uninstall the Software from its computer system and return to Licensor all copies of the Software, together with the Documentation for the Software and all other material containing information concerning the Software which has either been supplied to it or of which it has become aware, whereupon Licensee\342\200\231s obligations under this Agreement shall cease, other than those under clause 4 of this Agreement. </span></p>\n"
"<"
                        "p style=\"-qt-paragraph-type:empty; margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">3</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\">\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-size:10pt;\">Licensee\342\200\231s obligations </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">3.1 \302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240 During the Evaluation Period Licensee shall: </"
                        "span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">(a)</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\">\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-size:10pt;\">Install and keep the Software installed on its computer system and ensure that the\302\240 Software is only used by its employees who would normally use such a product. </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Arial'; font-size:10pt;\">(b)</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\">\302\240\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-family:'Arial'; font-size:10pt;\">Provide Licensor with verbal and written reports on the Software\342\200\231s performance</span><span style=\" font-family:'"
                        "Arial'; font-size:10pt; font-style:italic;\">. </span><span style=\" font-family:'Arial'; font-size:10pt;\">\302\240In addition the reports shall identify any errors, bugs or shortcomings that limit the usability of the Software.\302\240 The Licensee shall also make those of its employees who are using the Software available for discussions with the Licensor from time to time.</span><span style=\" font-size:10pt;\"> </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Arial'; font-size:10pt;\">\302\240</span><span style=\" font-size:10pt;\"> </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">4</span><span st"
                        "yle=\" font-family:'Times New Roman'; font-size:10pt;\">\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-size:10pt;\">Confidentiality </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">4.1 \302\240\302\240\302\240\302\240\302\240 During and after the Evaluation Period, Licensee shall treat the Software, Documentation and all information concerning it which is either supplied to it or of which it becomes aware as confidential and accordingly shall not: </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">(a)\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240 disclose any such information to any third party; or </span></p>\n"
"<p style=\" margin-top:12px; margin-bott"
                        "om:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">(b)\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240 disclose any such information to any employee who has not acknowledged in writing the confidentiality of such information; or </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">(c)\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240 use any such information other than for the purpose of its own internal evaluation, testing and evaluation of the Software except to the extent that such information is or becomes public knowledge other than through any fault of Licensee and shall at the request of Licensor and at its own cost take such proceedings as may be necessary to preserve the confidentiality of such information. </spa"
                        "n></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">\302\240 </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Arial'; font-size:10pt;\">5</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\">\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-family:'Arial'; font-size:10pt; font-weight:600;\">References to Licensee\342\200\231s use of the Software and publication of results</span><span style=\" font-size:10pt;\"> </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Arial'; font-size:10pt;\">UCL shall be permitted to publish the results of the Licensee\342\200\231s evaluation of the Software. All pro"
                        "posed publications containing results relating to the evaluation shall be submitted in writing to the Licensee for review at least fourteen (14) days before submission for publication or presentation. The Licensee may request amendment to the publication through which any commercially sensitive background intellectual property of the Licensee is disguised to the satisfaction the Licensee. If the Licensee fails to notify UCL of any required amendments within 14 days of receiving the proposed publication, then UCL shall be free to assume that the Licensee has no objection to the proposed publication. </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">6</span><span style=\" font-family:'Times New Roman'; font-size:10pt"
                        ";\">\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-size:10pt;\">Intellectual Property Rights </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Copyright and all other intellectual property rights in the Software and the Documentation shall remain at all times the property of Licensor and Licensee shall acquire no rights in any such material except as expressly provided in this Agreement. </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">7</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\">\302\240\302\240\302\240\302"
                        "\240\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-size:10pt;\">Exclusion of warranty </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Licensee acknowledges that during the Evaluation Period the Software will be for test and evaluation purposes only, is being provided \342\200\230AS IS\342\200\231 without any warranty of any kind and is being tested and evaluated by Licensee at its own risk. </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">8</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\">\302\240\302\240\302\240\302\240\302\240\302\240\302\240"
                        "\302\240\302\240\302\240 </span><span style=\" font-size:10pt;\">General </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">8.1</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\">\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-size:10pt;\">Licensee may not assign its rights and obligations under this Agreement. </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">8.2</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\">\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-size:10pt;\">In the event that any or any part of the terms, conditions or provisions contained in this Agreement are determined by any competent authority to be invalid, unlawful or unenforceable to any extent such term, "
                        "condition or provision shall to that extent be severed from the remaining terms, conditions and provisions which shall continue to be valid and enforceable to the fullest extent permitted. </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">8.3</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\">\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-size:10pt;\">This Agreement shall be governed by and construed in accordance with the laws of England and Wales to the non-exclusive jurisdiction of the courts of which the parties hereby submit. </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
                        "<span style=\" font-size:10pt;\">9</span><span style=\" font-family:'Times New Roman'; font-size:10pt;\">\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240\302\240 </span><span style=\" font-size:10pt;\">Third parties </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">This Agreement does not create any right enforceable by any person who is not a party to it ('Third Party') under the Contracts (Rights of Third Parties) Act 1999, but this clause does not affect any right or remedy of a Third Party which exists or is available apart from that Act. </span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("LicenseAgreement", "Click \"OK\" to Accept the License or Cancel to Exit...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LicenseAgreement: public Ui_LicenseAgreement {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LICENSEAGREEMENT_H
