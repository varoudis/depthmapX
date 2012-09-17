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

#include "ConvertShapesDlg.h"

CConvertShapesDlg::CConvertShapesDlg(QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	m_radius = 10.0;
	m_selected_only = false;
	m_conversion_type = 0;
	UpdateData(false);
}

void CConvertShapesDlg::OnOK()
{
	UpdateData(true);
	accept();
}

void CConvertShapesDlg::OnCancel()
{
	reject();
}

void CConvertShapesDlg::UpdateData(bool value)
{
    double m_radius = 0.0;
	if (value)
	{
        m_radius = c_radius->text().toDouble();
		if (c_selected_only->checkState())
			m_selected_only = true;
		else
			m_selected_only = false;

		m_conversion_type = c_conversion_type->currentIndex();
	}
	else
	{
		c_radius->setText(QString("%1").arg(m_radius));
		if (m_selected_only)
			c_selected_only->setCheckState(Qt::Checked);
		else
			c_selected_only->setCheckState(Qt::Unchecked);

		c_conversion_type->setCurrentIndex(m_conversion_type);
	}
}

void CConvertShapesDlg::showEvent(QShowEvent *event)
{
	UpdateData(false);
}

