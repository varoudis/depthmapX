/********************************************************************************
** Form generated from reading UI file 'ColumnPropertiesDlg.ui'
**
** Created: Mon Apr 16 10:38:40 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLUMNPROPERTIESDLG_H
#define UI_COLUMNPROPERTIESDLG_H

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
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CColumnPropertiesDlg
{
public:
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QLabel *c_name_text;
    QLineEdit *c_name;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QTableWidget *c_summary;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QTextEdit *c_formula;
    QLabel *c_formula_note;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;

    void setupUi(QDialog *CColumnPropertiesDlg)
    {
        if (CColumnPropertiesDlg->objectName().isEmpty())
            CColumnPropertiesDlg->setObjectName(QString::fromUtf8("CColumnPropertiesDlg"));
        CColumnPropertiesDlg->resize(440, 565);
        verticalLayout_4 = new QVBoxLayout(CColumnPropertiesDlg);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        c_name_text = new QLabel(CColumnPropertiesDlg);
        c_name_text->setObjectName(QString::fromUtf8("c_name_text"));

        verticalLayout_3->addWidget(c_name_text);

        c_name = new QLineEdit(CColumnPropertiesDlg);
        c_name->setObjectName(QString::fromUtf8("c_name"));
        c_name->setReadOnly(true);

        verticalLayout_3->addWidget(c_name);


        verticalLayout_4->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(CColumnPropertiesDlg);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        c_summary = new QTableWidget(CColumnPropertiesDlg);
        c_summary->setObjectName(QString::fromUtf8("c_summary"));

        verticalLayout_2->addWidget(c_summary);


        verticalLayout_4->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_2 = new QLabel(CColumnPropertiesDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        c_formula = new QTextEdit(CColumnPropertiesDlg);
        c_formula->setObjectName(QString::fromUtf8("c_formula"));
        c_formula->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        c_formula->setReadOnly(true);

        verticalLayout->addWidget(c_formula);


        verticalLayout_4->addLayout(verticalLayout);

        c_formula_note = new QLabel(CColumnPropertiesDlg);
        c_formula_note->setObjectName(QString::fromUtf8("c_formula_note"));

        verticalLayout_4->addWidget(c_formula_note);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(248, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        c_ok = new QPushButton(CColumnPropertiesDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout->addWidget(c_ok);


        verticalLayout_4->addLayout(horizontalLayout);


        retranslateUi(CColumnPropertiesDlg);
        QObject::connect(c_ok, SIGNAL(clicked()), CColumnPropertiesDlg, SLOT(OnOK()));

        QMetaObject::connectSlotsByName(CColumnPropertiesDlg);
    } // setupUi

    void retranslateUi(QDialog *CColumnPropertiesDlg)
    {
        CColumnPropertiesDlg->setWindowTitle(QApplication::translate("CColumnPropertiesDlg", "Attribute Properties", 0, QApplication::UnicodeUTF8));
        c_name_text->setText(QApplication::translate("CColumnPropertiesDlg", "Name", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CColumnPropertiesDlg", "Values", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CColumnPropertiesDlg", "Formula", 0, QApplication::UnicodeUTF8));
        c_formula_note->setText(QApplication::translate("CColumnPropertiesDlg", "Note: the formula may have been applied to a subset of objects", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CColumnPropertiesDlg", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CColumnPropertiesDlg: public Ui_CColumnPropertiesDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLUMNPROPERTIESDLG_H
