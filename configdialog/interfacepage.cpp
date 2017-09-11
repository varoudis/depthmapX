// Copyright (C) 2017 Petros Koutsolampros

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "interfacepage.h"
#include <QtWidgets>
#include <QColorDialog>

InterfacePage::InterfacePage(Settings &settings, QWidget *parent)
    : SettingsPage(settings, parent)
{
    readSettings(settings);
    QGroupBox *interfaceColoursGroup = new QGroupBox(tr("Interface colours"));
    QListWidget *interfaceColoursList = new QListWidget;
    connect(interfaceColoursList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this, SLOT(onInterfaceColourlItemClicked(QListWidgetItem*)));

    QListWidgetItem *bgItem = new QListWidgetItem(interfaceColoursList);
    bgItem->setText(tr("Background"));
    QPixmap pixmap(20,20);
    pixmap.fill(m_background);
    QIcon bgColourIcon(pixmap);
    bgItem->setIcon(bgColourIcon);
    colourMap.insert(std::pair<QListWidgetItem *, QColor *>(bgItem, &m_background));

    QListWidgetItem *fgItem = new QListWidgetItem(interfaceColoursList);
    fgItem->setText(tr("Foreground"));
    pixmap.fill(m_foreground);
    QIcon fgColourIcon(pixmap);
    fgItem->setIcon(fgColourIcon);
    colourMap.insert(std::pair<QListWidgetItem *, QColor *>(fgItem, &m_foreground));

    QVBoxLayout *interfaceColoursLayout = new QVBoxLayout;
    interfaceColoursLayout->addWidget(interfaceColoursList);
    interfaceColoursGroup->setLayout(interfaceColoursLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(interfaceColoursGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

void InterfacePage::onInterfaceColourlItemClicked(QListWidgetItem* item)
{
    QColor colour = QColorDialog::getColor();
    colourMap[item]->setRed(colour.red());
    colourMap[item]->setGreen(colour.green());
    colourMap[item]->setBlue(colour.blue());
    QPixmap pixmap(100,100);
    pixmap.fill(colour);
    QIcon bgColourIcon(pixmap);
    item->setIcon(bgColourIcon);
}
