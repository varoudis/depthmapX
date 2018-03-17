// Copyright (C) 2011-2012, Tasos Varoudis

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


#include <QtCore/QEvent>
#include <QtCore/QDebug>

#include <QtWidgets/QMenu>
#include <QtWidgets/QLayout>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidgetItem>
#include <QtGui/QFocusEvent>

#include "treeWindow.h"

QT_BEGIN_NAMESPACE

indexWidget::indexWidget(QWidget *parent, bool custom)
    : QTreeWidget(parent)
    , m_custom(custom)
{
    if (m_custom) {
        setColumnCount(3);
        hideColumn(1);
        QStringList columnNames(tr("Map"));
        columnNames << tr("Folder");
        columnNames << tr("Editable");
        setHeaderLabels(columnNames);
        header()->setSectionResizeMode(0, QHeaderView::Stretch);
        header()->resizeSection(0, 200);
        header()->resizeSection(1, 10);
        header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
        header()->resizeSection(2, 10);
        header()->setStretchLastSection(false);
    } else {
        header()->hide();
    }

    installEventFilter(this);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), parent,
        SLOT(OnSelchangingTree(QTreeWidgetItem*, int)));
}

indexWidget::~indexWidget()
{
    // nothing todo
}

void indexWidget::removeAllItem(QTreeWidgetItem *start)
{
    int index;
    QTreeWidgetItem *currentItem = start;
    if(currentItem) //while (currentItem) 
	{
        QTreeWidgetItem *parent = currentItem->parent();
        if (parent) {
            index = parent->indexOfChild(currentItem);
            delete parent->takeChild(index);
        } else {
            index = indexOfTopLevelItem(currentItem);
            delete takeTopLevelItem(index);
        }
    }
}

QTreeWidgetItem* indexWidget::addNewRootFolder(const QString &title)
{
    QString folderName = title;
    QTreeWidgetItem *newItem = 0;
    
    QStringList columnStrings(folderName);
    columnStrings << QLatin1String("Folder");
    newItem = new QTreeWidgetItem(this, columnStrings);

    setCurrentItem(newItem);
	return newItem;
}

QTreeWidgetItem* indexWidget::addNewFolder(const QString &title, QTreeWidgetItem *parent)
{
    QString folderName = title;

    QTreeWidgetItem *newItem = 0;
    QTreeWidgetItem *treeItem = itemIfNotDirectory();
    
    QStringList columnStrings(folderName);
    columnStrings << QLatin1String("Folder");
	if(parent)
	{
        newItem = new QTreeWidgetItem(parent, columnStrings);
	}
    else if (treeItem) {
        newItem = new QTreeWidgetItem(columnStrings);
        treeItem->addChild(newItem);
    } 
	else {
        newItem = new QTreeWidgetItem(this, columnStrings);
    }

    setCurrentItem(newItem);
    newItem->setFlags(newItem->flags() &~ (Qt::ItemIsEditable | Qt::ItemIsSelectable));
	return newItem;
}

QTreeWidgetItem * indexWidget::addNewItem(const QString &title, const QString &url)
{
    QTreeWidgetItem *newItem = 0;
    QTreeWidgetItem *treeItem = itemIfNotDirectory();

    QStringList columnStrings(title);
    if (treeItem) {
        newItem = new QTreeWidgetItem(columnStrings);// << url);
        treeItem->insertChild(0, newItem);
    } else {
        newItem = new QTreeWidgetItem(this, columnStrings);// << url);
    }

    setCurrentItem(newItem);
    newItem->setFlags(newItem->flags() &~ (Qt::ItemIsEditable | Qt::ItemIsSelectable));
	return newItem;

	if(url == "") return NULL;
}


QTreeWidgetItem* indexWidget::itemIfNotDirectory()
{
    QTreeWidgetItem *currentItem = this->currentItem();

    if (currentItem) {
        QString data = currentItem->data(1, Qt::DisplayRole).toString();
        if (data != QLatin1String("Folder"))
            currentItem = currentItem->parent();
    }
	return currentItem;
}


QT_END_NAMESPACE
