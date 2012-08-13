/********************************************************************************
** Form generated from reading UI file 'FilePropertiesDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEPROPERTIESDLG_H
#define UI_FILEPROPERTIESDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CFilePropertiesDlg
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *c_title;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *c_location;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QSpacerItem *verticalSpacer;
    QTextEdit *c_description;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *c_author;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *c_organization;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QLineEdit *c_create_date;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QLineEdit *c_create_program;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_8;
    QLineEdit *c_file_version;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CFilePropertiesDlg)
    {
        if (CFilePropertiesDlg->objectName().isEmpty())
            CFilePropertiesDlg->setObjectName(QString::fromUtf8("CFilePropertiesDlg"));
        CFilePropertiesDlg->resize(400, 430);
        verticalLayout_2 = new QVBoxLayout(CFilePropertiesDlg);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CFilePropertiesDlg);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        c_title = new QLineEdit(CFilePropertiesDlg);
        c_title->setObjectName(QString::fromUtf8("c_title"));

        horizontalLayout->addWidget(c_title);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(CFilePropertiesDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        c_location = new QLineEdit(CFilePropertiesDlg);
        c_location->setObjectName(QString::fromUtf8("c_location"));

        horizontalLayout_2->addWidget(c_location);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_3 = new QLabel(CFilePropertiesDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_3->addLayout(verticalLayout);

        c_description = new QTextEdit(CFilePropertiesDlg);
        c_description->setObjectName(QString::fromUtf8("c_description"));

        horizontalLayout_3->addWidget(c_description);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(CFilePropertiesDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        c_author = new QLineEdit(CFilePropertiesDlg);
        c_author->setObjectName(QString::fromUtf8("c_author"));
        c_author->setReadOnly(true);

        horizontalLayout_4->addWidget(c_author);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(CFilePropertiesDlg);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_5->addWidget(label_5);

        c_organization = new QLineEdit(CFilePropertiesDlg);
        c_organization->setObjectName(QString::fromUtf8("c_organization"));
        c_organization->setReadOnly(true);

        horizontalLayout_5->addWidget(c_organization);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_6 = new QLabel(CFilePropertiesDlg);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_6->addWidget(label_6);

        c_create_date = new QLineEdit(CFilePropertiesDlg);
        c_create_date->setObjectName(QString::fromUtf8("c_create_date"));
        c_create_date->setReadOnly(true);

        horizontalLayout_6->addWidget(c_create_date);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_7 = new QLabel(CFilePropertiesDlg);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_7->addWidget(label_7);

        c_create_program = new QLineEdit(CFilePropertiesDlg);
        c_create_program->setObjectName(QString::fromUtf8("c_create_program"));
        c_create_program->setReadOnly(true);

        horizontalLayout_7->addWidget(c_create_program);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_8 = new QLabel(CFilePropertiesDlg);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_8->addWidget(label_8);

        c_file_version = new QLineEdit(CFilePropertiesDlg);
        c_file_version->setObjectName(QString::fromUtf8("c_file_version"));
        c_file_version->setReadOnly(true);

        horizontalLayout_8->addWidget(c_file_version);


        verticalLayout_2->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer);

        c_ok = new QPushButton(CFilePropertiesDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout_9->addWidget(c_ok);

        c_cancel = new QPushButton(CFilePropertiesDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout_9->addWidget(c_cancel);


        verticalLayout_2->addLayout(horizontalLayout_9);


        retranslateUi(CFilePropertiesDlg);
        QObject::connect(c_ok, SIGNAL(clicked()), CFilePropertiesDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CFilePropertiesDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CFilePropertiesDlg);
    } // setupUi

    void retranslateUi(QDialog *CFilePropertiesDlg)
    {
        CFilePropertiesDlg->setWindowTitle(QApplication::translate("CFilePropertiesDlg", "Graph File Properties", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CFilePropertiesDlg", "Title", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CFilePropertiesDlg", "Location", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CFilePropertiesDlg", "Description", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CFilePropertiesDlg", "Author", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("CFilePropertiesDlg", "Organization", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CFilePropertiesDlg", "Created on", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("CFilePropertiesDlg", "Created by", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("CFilePropertiesDlg", "File version", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CFilePropertiesDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CFilePropertiesDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CFilePropertiesDlg: public Ui_CFilePropertiesDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEPROPERTIESDLG_H
