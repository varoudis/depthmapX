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


#ifndef treeWindow_H
#define treeWindow_H
#include <QListWidget>

QT_BEGIN_NAMESPACE

class QEvent;
class QListWidgetItem;

class AttribWindow : public QListWidget
{
    Q_OBJECT

public:
    AttribWindow(QWidget *parent = 0, bool custom = true);
    ~AttribWindow();

	QWidget* main_frm;

private slots:
    void showContextMenu(const QPoint &point);
};

QT_END_NAMESPACE

#endif
