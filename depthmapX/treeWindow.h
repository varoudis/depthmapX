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

class indexWidget : public QTreeWidget
{
    Q_OBJECT

public:
    indexWidget(QWidget *parent = 0, bool custom = true);
    ~indexWidget();

signals:
    void requestShowLink(const QUrl& url);

public slots:
	void removeAllItem(QTreeWidgetItem *start);
	QTreeWidgetItem* addNewRootFolder(const QString &title);
    QTreeWidgetItem * addNewFolder(const QString &title, QTreeWidgetItem *parent = 0);
    QTreeWidgetItem * addNewItem(const QString& title, const QString &url);

private:
    QTreeWidgetItem* itemIfNotDirectory();

private:
    bool m_custom;
};

QT_END_NAMESPACE

#endif  // BOOKMARK_WIDGET_H
