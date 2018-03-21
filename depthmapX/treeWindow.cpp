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

IndexWidget::IndexWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    setColumnCount(2);
    setHeaderLabels(columnNames);
    header()->setSectionResizeMode(columnIndex(m_mapColumn), QHeaderView::Stretch);
    header()->setSectionResizeMode(columnIndex(m_editableColumn), QHeaderView::ResizeToContents);
    header()->resizeSection(columnIndex(m_editableColumn), 10);
    header()->setStretchLastSection(false);

    installEventFilter(this);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), parent,
        SLOT(OnSelchangingTree(QTreeWidgetItem*, int)));
}

void IndexWidget::removeAllItem(QTreeWidgetItem *start)
{
    int index;
    QTreeWidgetItem *currentItem = start;
    if(currentItem)
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

QTreeWidgetItem * IndexWidget::addNewItem(const QString &title, QTreeWidgetItem* parent)
{
    QTreeWidgetItem *newItem = 0;

    QStringList columnStrings(title);
    if (parent != NULL) {
        newItem = new QTreeWidgetItem(parent, columnStrings);
    } else {
        newItem = new QTreeWidgetItem(this, columnStrings);
    }

    setCurrentItem(newItem);
    newItem->setFlags(newItem->flags() &~ (Qt::ItemIsEditable | Qt::ItemIsSelectable));
	return newItem;
}


QT_END_NAMESPACE
