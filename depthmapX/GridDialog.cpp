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

#include "GridDialog.h"
#include <QMessageBox>

CGridDialog::CGridDialog(QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	m_spacing = 0.01;
	m_maxdimension = 1.0;
}

void CGridDialog::showEvent(QShowEvent * event)
{
	m_maxexponent = (int) floor(log10(m_maxdimension)) - 1;
	m_minexponent = m_maxexponent - 2;
	m_basemantissa = (int) floor(m_maxdimension / pow(10.0,double(m_maxexponent+1)));

	// current:
	m_mantissa = m_basemantissa;
	m_exponent = m_maxexponent - 1;

	m_spacing = (double) m_mantissa * pow(10.0, double(m_exponent));

    double truemax = (double) 2 * m_mantissa * pow(10.0, double(m_maxexponent));
    double truemin = (double) m_mantissa * pow(10.0, double(m_minexponent));
    c_spacing_ctrl->setRange(truemin, truemax);

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

	double truemax = (double) 2 * m_mantissa * pow(10.0, double(m_maxexponent));
	double truemin = (double) m_mantissa * pow(10.0, double(m_minexponent));
	if (m_spacing > truemax || m_spacing < truemin) {
		QString formatabsmin, formatmin, formatmax;
		if (m_minexponent < 0) {
			formatmin.sprintf("%%.%df", abs(m_minexponent));
		}
		else {
			formatmin = tr("%.0f");
		}
		if (m_minexponent-1 < 0) {
			formatabsmin.sprintf("%%.%df", abs(m_minexponent-1));
		}
		else {
			formatabsmin = tr("%.0f");
		}
		if (m_maxexponent < 0) {
			formatmax.sprintf("%%.%df" ,abs(m_maxexponent));
		}
		else {
			formatmax = tr("%.0f");
		}
		QString absminstr, minstr, maxstr;
		absminstr.sprintf(formatabsmin.toAscii(), truemin/10);
		minstr.sprintf(formatmin.toAscii(), truemin);
		maxstr.sprintf(formatmax.toAscii(), truemax);
		if (m_spacing >= truemin / 10 && m_spacing < truemin) {
			QString msg;
			msg = tr("You are below the suggested minimum grid spacing of ") + minstr + tr(".  If you use this grid spacing, it may cause processing problems.\nAre you sure you want to proceed with this grid spacing?");
			if (QMessageBox::No == QMessageBox::question(this, tr("Question"), msg, QMessageBox::Yes | QMessageBox::No)) {
				return;
			}
		}
		else {
			QString msg;
			msg = tr("Please enter a spacing between ") + absminstr + tr(" (at the absolute minimum) and ") + maxstr;
			QMessageBox::warning(this, "Notice", msg, QMessageBox::Ok, QMessageBox::Ok);
			return;
		}
	}

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

void CGridDialog::userValue(double value)
{
    m_spacing = c_spacing_ctrl->value();
}
