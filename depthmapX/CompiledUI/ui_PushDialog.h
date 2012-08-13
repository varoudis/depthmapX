/********************************************************************************
** Form generated from reading UI file 'PushDialog.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PUSHDIALOG_H
#define UI_PUSHDIALOG_H

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

class Ui_CPushDialog
{
public:
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *c_origin_layer;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QLineEdit *c_origin_attribute;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QComboBox *c_layer_selector;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QComboBox *c_function;
    QCheckBox *c_count_intersections;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CPushDialog)
    {
        if (CPushDialog->objectName().isEmpty())
            CPushDialog->setObjectName(QString::fromUtf8("CPushDialog"));
        CPushDialog->resize(337, 329);
        verticalLayout_5 = new QVBoxLayout(CPushDialog);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(CPushDialog);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        c_origin_layer = new QLineEdit(CPushDialog);
        c_origin_layer->setObjectName(QString::fromUtf8("c_origin_layer"));
        c_origin_layer->setReadOnly(true);

        verticalLayout->addWidget(c_origin_layer);


        verticalLayout_5->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(CPushDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        c_origin_attribute = new QLineEdit(CPushDialog);
        c_origin_attribute->setObjectName(QString::fromUtf8("c_origin_attribute"));
        c_origin_attribute->setReadOnly(true);

        verticalLayout_2->addWidget(c_origin_attribute);


        verticalLayout_5->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_3 = new QLabel(CPushDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_3->addWidget(label_3);

        c_layer_selector = new QComboBox(CPushDialog);
        c_layer_selector->setObjectName(QString::fromUtf8("c_layer_selector"));

        verticalLayout_3->addWidget(c_layer_selector);


        verticalLayout_5->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_4 = new QLabel(CPushDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_4->addWidget(label_4);

        c_function = new QComboBox(CPushDialog);
        c_function->setObjectName(QString::fromUtf8("c_function"));

        verticalLayout_4->addWidget(c_function);


        verticalLayout_5->addLayout(verticalLayout_4);

        c_count_intersections = new QCheckBox(CPushDialog);
        c_count_intersections->setObjectName(QString::fromUtf8("c_count_intersections"));

        verticalLayout_5->addWidget(c_count_intersections);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        c_ok = new QPushButton(CPushDialog);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout->addWidget(c_ok);

        c_cancel = new QPushButton(CPushDialog);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout->addWidget(c_cancel);


        verticalLayout_5->addLayout(horizontalLayout);


        retranslateUi(CPushDialog);
        QObject::connect(c_ok, SIGNAL(clicked()), CPushDialog, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CPushDialog, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CPushDialog);
    } // setupUi

    void retranslateUi(QDialog *CPushDialog)
    {
        CPushDialog->setWindowTitle(QApplication::translate("CPushDialog", "Push Values to Map", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CPushDialog", "Origin map", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CPushDialog", "Origin attribute", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CPushDialog", "Push values to", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CPushDialog", "If destination object intersects more than\n"
" one object in origin map", 0, QApplication::UnicodeUTF8));
        c_function->clear();
        c_function->insertItems(0, QStringList()
         << QApplication::translate("CPushDialog", "Take maximum attribute value", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CPushDialog", "Take minimum attribute value", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CPushDialog", "Take average attribute value", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CPushDialog", "Take total of attribute values", 0, QApplication::UnicodeUTF8)
        );
        c_count_intersections->setText(QApplication::translate("CPushDialog", "Record object intersection count", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CPushDialog", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CPushDialog", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CPushDialog: public Ui_CPushDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PUSHDIALOG_H
