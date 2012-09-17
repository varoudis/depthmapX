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

#include "IsovistPathDlg.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif


CIsovistPathDlg::CIsovistPathDlg(QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	fov_selection = 0;
	fov_angle = 0.0;
	UpdateData(false);
}

void CIsovistPathDlg::OnOK()
{
	UpdateData(true);

	switch (fov_selection) {
	  case 0:
		  fov_angle = M_PI * 0.5;
		  break;
	  case 1:
		  fov_angle = 2.0 * M_PI / 3.0;
		  break;
	  case 2:
		  fov_angle = M_PI;
		  break;
	  case 3:
		  fov_angle = 2.0 * M_PI;
		  break;
	  default:
		  fov_angle = 2.0 * M_PI;
		  break;
	}
	accept();
}

void CIsovistPathDlg::OnCancel()
{
	reject();
}

void CIsovistPathDlg::UpdateData(bool value)
{
	if (value)
	{
		fov_selection = c_fov_selection->currentIndex();
	}
	else
	{
		c_fov_selection->setCurrentIndex(fov_selection);
	}
}

void CIsovistPathDlg::showEvent(QShowEvent * event)
{
	UpdateData(false);
}
