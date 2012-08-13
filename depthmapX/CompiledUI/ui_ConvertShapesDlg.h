/********************************************************************************
** Form generated from reading UI file 'ConvertShapesDlg.ui'
**
** Created: Mon Apr 2 12:47:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONVERTSHAPESDLG_H
#define UI_CONVERTSHAPESDLG_H

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

class Ui_CConvertShapesDlg
{
public:
    QVBoxLayout *verticalLayout;
    QComboBox *c_conversion_type;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *c_radius;
    QCheckBox *c_selected_only;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *c_ok;
    QPushButton *c_cancel;

    void setupUi(QDialog *CConvertShapesDlg)
    {
        if (CConvertShapesDlg->objectName().isEmpty())
            CConvertShapesDlg->setObjectName(QString::fromUtf8("CConvertShapesDlg"));
        CConvertShapesDlg->resize(320, 180);
        verticalLayout = new QVBoxLayout(CConvertShapesDlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        c_conversion_type = new QComboBox(CConvertShapesDlg);
        c_conversion_type->setObjectName(QString::fromUtf8("c_conversion_type"));

        verticalLayout->addWidget(c_conversion_type);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(CConvertShapesDlg);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        c_radius = new QLineEdit(CConvertShapesDlg);
        c_radius->setObjectName(QString::fromUtf8("c_radius"));

        horizontalLayout->addWidget(c_radius);


        verticalLayout->addLayout(horizontalLayout);

        c_selected_only = new QCheckBox(CConvertShapesDlg);
        c_selected_only->setObjectName(QString::fromUtf8("c_selected_only"));

        verticalLayout->addWidget(c_selected_only);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        c_ok = new QPushButton(CConvertShapesDlg);
        c_ok->setObjectName(QString::fromUtf8("c_ok"));

        horizontalLayout_2->addWidget(c_ok);

        c_cancel = new QPushButton(CConvertShapesDlg);
        c_cancel->setObjectName(QString::fromUtf8("c_cancel"));

        horizontalLayout_2->addWidget(c_cancel);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(CConvertShapesDlg);
        QObject::connect(c_ok, SIGNAL(clicked()), CConvertShapesDlg, SLOT(OnOK()));
        QObject::connect(c_cancel, SIGNAL(clicked()), CConvertShapesDlg, SLOT(OnCancel()));

        QMetaObject::connectSlotsByName(CConvertShapesDlg);
    } // setupUi

    void retranslateUi(QDialog *CConvertShapesDlg)
    {
        CConvertShapesDlg->setWindowTitle(QApplication::translate("CConvertShapesDlg", "Convert Map Shapes", 0, QApplication::UnicodeUTF8));
        c_conversion_type->clear();
        c_conversion_type->insertItems(0, QStringList()
         << QApplication::translate("CConvertShapesDlg", "Convert points to polygons;", 0, QApplication::UnicodeUTF8)
        );
        label->setText(QApplication::translate("CConvertShapesDlg", "Polygon radius", 0, QApplication::UnicodeUTF8));
        c_selected_only->setText(QApplication::translate("CConvertShapesDlg", "Apply to selected shapes only", 0, QApplication::UnicodeUTF8));
        c_ok->setText(QApplication::translate("CConvertShapesDlg", "OK", 0, QApplication::UnicodeUTF8));
        c_cancel->setText(QApplication::translate("CConvertShapesDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CConvertShapesDlg: public Ui_CConvertShapesDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONVERTSHAPESDLG_H
