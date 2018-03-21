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


#ifndef indexWidget_H
#define indexWidget_H

#include <QTreeWidget>

QT_BEGIN_NAMESPACE

class QEvent;
class QTreeWidgetItem;

class IndexWidget : public QTreeWidget
{
    Q_OBJECT
private:
    enum Column {MAP = 0, EDITABLE = 1};

public:
    IndexWidget(QWidget *parent = 0);

    QString m_mapColumn = "Map";
    QString m_editableColumn = "Editable";

    void setItemVisibility(QTreeWidgetItem* item, Qt::CheckState checkState) {
        item->setCheckState(Column::MAP, checkState);
    }
    void setItemEditability(QTreeWidgetItem* item, Qt::CheckState checkState) {
        item->setCheckState(Column::EDITABLE, checkState);
    }
    void setItemReadOnly(QTreeWidgetItem* item) {
        item->setData(Column::EDITABLE, Qt::CheckStateRole, QVariant());
    }
    bool isItemSetVisible(QTreeWidgetItem* item) {
        return item->checkState(Column::MAP);
    }
    bool isItemSetEditable(QTreeWidgetItem* item) {
        return item->checkState(Column::EDITABLE);
    }
    bool isMapColumn(int col) {
        return col == Column::MAP;
    }
    bool isEditableColumn(int col) {
        return col == Column::EDITABLE;
    }

signals:
    void requestShowLink(const QUrl& url);

public slots:
    void removeAllItem(QTreeWidgetItem *start);
    QTreeWidgetItem * addNewItem(const QString& title, QTreeWidgetItem *parent = NULL);

private:
    QStringList columnNames = (QStringList()
                               << m_mapColumn
                               << m_editableColumn);
};

QT_END_NAMESPACE

#endif  // BOOKMARK_WIDGET_H
