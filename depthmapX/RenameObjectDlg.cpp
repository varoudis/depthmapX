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

#include "RenameObjectDlg.h"

CRenameObjectDlg::CRenameObjectDlg(const QString& object_type, const QString& existing_name, QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	m_prompt = tr("");
	m_object_name = tr("");
	m_object_name = existing_name;

	m_object_type = object_type; // e.g., Column, Layer, etc
}

void CRenameObjectDlg::OnOK()
{
	UpdateData(true);
	accept();
}

void CRenameObjectDlg::OnCancel()
{
	reject();
}

void CRenameObjectDlg::UpdateData(bool value)
{
	if (value)
	{
		m_object_name = c_object_name->text();
		m_prompt = c_prompt->text();
	}
	else
	{
		c_object_name->setText(m_object_name);
		c_prompt->setText(m_prompt);
	}
}

void CRenameObjectDlg::showEvent(QShowEvent * event)
{
	QString lower_object_type = m_object_type;
	lower_object_type = lower_object_type.toLower();

	if (m_object_name.isEmpty()) {
		QString title = QString("New ") + m_object_type;
		setWindowTitle(title);
		m_prompt = QString("New ") + lower_object_type + QString(" name:");
		m_object_name = QString("<New ") + m_object_type + QString(">");
	}
	else {
		QString title = QString("Rename ") + m_object_type;
		setWindowTitle(title);
		m_prompt = QString("Rename ") + lower_object_type + QString(" to:");
	}
	UpdateData(false);
}
