// Copyright (C) 2011-2012, Tasos Varoudis
// Copyright (C) 2017 Christian Sailer

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

#include "GridDialog.h"
#include <QMessageBox>
#include <salalib/gridproperties.h>

CGridDialog::CGridDialog(double maxDimension, QWidget *parent)
: QDialog(parent), m_maxdimension(maxDimension)
{
	setupUi(this);
	m_spacing = 0.01;
}

void CGridDialog::showEvent(QShowEvent * event)
{
    GridProperties gp(m_maxdimension);
    m_spacing = gp.getDefault();

    c_spacing_ctrl->setRange(gp.getMin(), gp.getMax());

	UpdateData(false);
}

void CGridDialog::OnDeltaposSpinSpacing(double iDelta)
{
   // New slot for this ready userValue(double value) // slot link here though // TV
   // m_spacing = c_spacing_ctrl->value(); // bug or not?

    if (int(iDelta / 1.0) > 1)	c_spacing_ctrl->setSingleStep(1);
    else if (int(iDelta / 0.1) > 1)	c_spacing_ctrl->setSingleStep(0.1);
    else if (int(iDelta / 0.01) > 1)	c_spacing_ctrl->setSingleStep(0.01);
    else	c_spacing_ctrl->setSingleStep(0.001);

}

void CGridDialog::OnOK()
{
	UpdateData(true);
	accept();
}

void CGridDialog::OnCancel()
{
	reject();
}

void CGridDialog::UpdateData(bool value)
{
	if (value)
	{
		m_spacing = c_spacing_ctrl->value();
	}
	else
	{
		c_spacing_ctrl->setValue(m_spacing);
	}
}
