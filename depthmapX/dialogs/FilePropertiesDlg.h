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

#include "ui_FilePropertiesDlg.h"

class CFilePropertiesDlg : public QDialog, public Ui::CFilePropertiesDlg
{
	Q_OBJECT
public:
	CFilePropertiesDlg(QWidget *parent = 0);
	QString	m_author;
	QString	m_create_date;
	QString	m_create_program;
	QString	m_description;
	QString	m_location;
	QString	m_organization;
	QString	m_title;
	QString	m_file_version;
	void UpdateData(bool value);
	void showEvent(QShowEvent * event);

private slots:
		void OnOK();
		void OnCancel();
};
