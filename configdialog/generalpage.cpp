
#include "generalpage.h"
#include <QtWidgets>
#include "depthmapX/settings.h"

GeneralPage::GeneralPage(Settings &settings, QWidget *parent)
    : SettingsPage(settings, parent)
{
    readSettings(settings);
    QGroupBox *configGroup = new QGroupBox(tr("General configuration"));
    QCheckBox *simpleModeCheckBox = new QCheckBox(tr("Simple mode"));
    simpleModeCheckBox->setChecked(m_simpleVersion);
    connect(simpleModeCheckBox, &QCheckBox::stateChanged, [=] () {m_simpleVersion = !m_simpleVersion;});

    QVBoxLayout *configLayout = new QVBoxLayout;
    configLayout->addWidget(simpleModeCheckBox);
    configGroup->setLayout(configLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(configGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}
