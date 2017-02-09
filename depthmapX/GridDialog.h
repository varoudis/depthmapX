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

#include "ui_GridDialog.h"
#include <salalib/mgraph.h>
#include <salalib/attributes.h>
#include <salalib/shapemap.h>
#include <salalib/axialmap.h>


class CGridDialog : public QDialog, public Ui::CGridDialog
{
	Q_OBJECT
public:
	CGridDialog(QWidget *parent = 0);
	double	m_spacing;
	double m_maxdimension;
	int m_minexponent;
	int m_maxexponent;
	int m_basemantissa;
	int m_mantissa;
	int m_exponent;
    void UpdateData(bool value);
	void showEvent(QShowEvent * event);

private slots:
		void OnDeltaposSpinSpacing(double);
		void OnOK();
		void OnCancel();
        void userValue(double value);
};
