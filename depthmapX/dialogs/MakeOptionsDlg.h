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

#include "ui_MakeOptionsDlg.h"

class CMakeOptionsDlg : public QDialog, public Ui::CMakeOptionsDlg
{
	Q_OBJECT
public:
	CMakeOptionsDlg(QWidget *parent = 0);
	bool	m_boundarygraph;
	double	m_maxdist;
	bool	m_restrict_visibility;
	void UpdateData(bool value);
	void showEvent(QShowEvent * event);

	private slots:
		void OnRestrict(bool);
		void OnOK();
		void OnCancel();
};
