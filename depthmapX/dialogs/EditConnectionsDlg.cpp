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

#include "EditConnectionsDlg.h"

CEditConnectionsDlg::CEditConnectionsDlg(QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	m_pin_to_sel = true;
	m_sel_to_pin = true;
	m_join_type = false;
	UpdateData(false);
}

void CEditConnectionsDlg::OnOK()
{
	UpdateData(true);
	accept();
}

void CEditConnectionsDlg::OnCancel()
{
	reject();
}

void CEditConnectionsDlg::UpdateData(bool value)
{
	if (value)
	{
		m_join_type = c_join_type->isChecked();
		if (c_sel_to_pin->checkState())
			m_sel_to_pin = true;
		else
			m_sel_to_pin = false;

		if (c_pin_to_sel->checkState())
			m_pin_to_sel = true;
		else
			m_pin_to_sel = false;
	}
	else
	{
		c_join_type->setChecked(m_join_type);
		if (m_sel_to_pin)
			c_sel_to_pin->setCheckState(Qt::Checked);
		else
			c_sel_to_pin->setCheckState(Qt::Unchecked);

		if (m_pin_to_sel)
			c_pin_to_sel->setCheckState(Qt::Checked);
		else
			c_pin_to_sel->setCheckState(Qt::Unchecked);
	}
}

void CEditConnectionsDlg::showEvent(QShowEvent *event)
{
	UpdateData(false);
}
