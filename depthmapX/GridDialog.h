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

#include "ui_GridDialog.h"

class CGridDialog : public QDialog, public Ui::CGridDialog
{
	Q_OBJECT
public:
    CGridDialog(double maxDimension, QWidget *parent = 0);
    void UpdateData(bool value);
	void showEvent(QShowEvent * event);
    double getSpacing() const { return m_spacing; }

private:
    double	m_spacing;
    double m_maxdimension;

private slots:
		void OnDeltaposSpinSpacing(double);
		void OnOK();
		void OnCancel();
};
