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

#include "FindLocDlg.h"
#include <QMessageBox>

CFindLocDlg::CFindLocDlg(QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	m_x = 0.0;
	m_y = 0.0;

	UpdateData(false);
}

void CFindLocDlg::OnOK()
{
	UpdateData(true);
	QPoint p((int)m_x, (int)m_y);
	if (!m_bounds.contains(p)) {
		QMessageBox::warning(this, tr("Warning"), tr("This point is outside the bounds of your map"), QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	accept();
}

void CFindLocDlg::OnCancel()
{
	reject();
}

void CFindLocDlg::UpdateData(bool value)
{
	if (value)
	{
		m_x = c_x->text().toDouble();
		m_y = c_y->text().toDouble();
	}
	else
	{
		c_x->setText(QString("%1").arg(m_x));
		c_y->setText(QString("%1").arg(m_y));
	}
}

void CFindLocDlg::showEvent(QShowEvent *event)
{
	UpdateData(false);
}
