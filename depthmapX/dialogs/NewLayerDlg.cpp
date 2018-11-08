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

#include "NewLayerDlg.h"
#include <QMessageBox>

CNewLayerDlg::CNewLayerDlg(QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	m_layer_type = 0;
	m_name = tr("Gate Map");

	UpdateData(false);
}

void CNewLayerDlg::OnSelchangeLayerType(int value)
{
	int which = value;

	switch (which) {
	case 0:
	   c_name->setText(tr("Gate Map"));
	   break;
	case 1:
	   c_name->setText(tr("Convex Map"));
	   break;
	case 2:
	   c_name->setText(tr("Axial Map"));
	   break;
	case 3:
	   c_name->setText(tr("Pesh Map"));
	   break;
	}
}

void CNewLayerDlg::OnOK()
{
	UpdateData(true);

	if (m_name.isEmpty()) {
		QMessageBox::warning(this, tr("Warning"), tr("Please enter a name for the new map"), QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	accept();
}

void CNewLayerDlg::OnCancel()
{
	reject();
}

void CNewLayerDlg::UpdateData(bool value)
{
	if (value)
	{
		m_layer_type = c_layer_selector->currentIndex();
		m_name = c_name->text();
	}
	else
	{
		c_layer_selector->setCurrentIndex(m_layer_type);
		c_name->setText(m_name);
	}
}

void CNewLayerDlg::showEvent(QShowEvent *event)
{
	UpdateData(false);
}
