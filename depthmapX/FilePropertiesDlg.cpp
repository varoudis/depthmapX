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

#include "FilePropertiesDlg.h"

CFilePropertiesDlg::CFilePropertiesDlg(QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	m_author = tr("");
	m_create_date = tr("");
	m_create_program = tr("");
	m_description = tr("");
	m_location = tr("");
	m_organization = tr("");
	m_title = tr("");
	m_file_version = tr("");

	UpdateData(false);
}

void CFilePropertiesDlg::OnOK()
{
	UpdateData(true);
	accept();
}

void CFilePropertiesDlg::OnCancel()
{
	reject();
}

void CFilePropertiesDlg::UpdateData(bool value)
{
	if (value)
	{
		m_author = c_author->text();
		m_create_date = c_create_date->text();
		m_create_program = c_create_program->text();
		m_description = c_description->toPlainText();
		m_location = c_location->text();
		m_organization = c_organization->text();
		m_title = c_title->text();
		m_file_version = c_file_version->text();
	}
	else
	{
		c_author->setText(m_author);
		c_create_date->setText(m_create_date);
		c_create_program->setText(m_create_program);
		c_description->setPlainText(m_description);
		c_location->setText(m_location);
		c_organization->setText(m_organization);
		c_title->setText(m_title);
		c_file_version->setText(m_file_version);
	}
}

void CFilePropertiesDlg::showEvent(QShowEvent *event)
{
	UpdateData(false);
}
