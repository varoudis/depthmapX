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

#include "MakeOptionsDlg.h"
#include <QMessageBox>

CMakeOptionsDlg::CMakeOptionsDlg(QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	m_boundarygraph = false;
	m_maxdist = 0.0;
	m_restrict_visibility = false;

	UpdateData(false);
}

void CMakeOptionsDlg::OnRestrict(bool)
{
	UpdateData(true);

	if (m_restrict_visibility) {
		c_maxdist->setEnabled(true);
	}
	else {
		c_maxdist->setEnabled(false);
	}
}

void CMakeOptionsDlg::OnOK()
{
	UpdateData(true);

	if (m_restrict_visibility && m_maxdist <= 0.0) {
		QMessageBox::warning(this, tr("Warning"), tr("Maximum distance must be over 0.0 if visibility is restricted"), QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	accept();
}

void CMakeOptionsDlg::OnCancel()
{
	reject();
}

void CMakeOptionsDlg::UpdateData(bool value)
{
	if (value)
	{
		if (c_boundarygraph->checkState())
			m_boundarygraph = true;
		else
			m_boundarygraph = false;

		m_maxdist = c_maxdist->text().toDouble();
		
		if (c_restrict_visibility->checkState())
			m_restrict_visibility = true;
		else
			m_restrict_visibility = false;
	}
	else
	{
		if (m_boundarygraph)
			c_boundarygraph->setCheckState(Qt::Checked);
		else
			c_boundarygraph->setCheckState(Qt::Unchecked);

		c_maxdist->setText(QString("%1").arg(m_maxdist));

		if (m_restrict_visibility)
			c_restrict_visibility->setCheckState(Qt::Checked);
		else
			c_restrict_visibility->setCheckState(Qt::Unchecked);
	}
}

void CMakeOptionsDlg::showEvent(QShowEvent * event)
{
	UpdateData(false);
}
