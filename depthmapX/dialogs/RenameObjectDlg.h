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

#include "ui_RenameObjectDlg.h"

class CRenameObjectDlg : public QDialog, public Ui::CRenameObjectDlg
{
	Q_OBJECT
public:
	CRenameObjectDlg(const QString& object_type, const QString& existing_name = QString(), QWidget *parent = 0);
	QString	m_object_name;
	QString m_object_type;
	QString m_prompt;
	void UpdateData(bool value);
	void showEvent(QShowEvent * event);

	private slots:
		void OnOK();
		void OnCancel();
};
