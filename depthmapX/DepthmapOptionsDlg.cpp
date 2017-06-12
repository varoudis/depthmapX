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

#include "DepthmapOptionsDlg.h"

CDepthmapOptionsDlg::CDepthmapOptionsDlg(QWidget *parent, bool simpleVersion)
: QDialog(parent), m_show_simple_version(simpleVersion)
{
	setupUi(this);
	m_show_research_toolbar = false;
}

void CDepthmapOptionsDlg::OnOK()
{
    //UpdateData(true);
	accept();
}

void CDepthmapOptionsDlg::OnCancel()
{
	reject();
}

void CDepthmapOptionsDlg::UpdateData(bool value)
{
	if (value)
	{
		if (m_show_research_toolbar)
			c_show_research_toolbar->setCheckState(Qt::Checked);
		else
			c_show_research_toolbar->setCheckState(Qt::Unchecked);

        if (m_show_simple_version)
            c_show_simple_version->setCheckState(Qt::Checked);
        else
            c_show_simple_version->setCheckState(Qt::Unchecked);
	}
	else
	{
		if (c_show_research_toolbar->checkState())
			m_show_research_toolbar = true;
		else
			m_show_research_toolbar = false;

        if (c_show_simple_version->checkState())
            m_show_simple_version = true;
        else
            m_show_simple_version = false;
	}
}

void CDepthmapOptionsDlg::showEvent(QShowEvent * event)
{
    //c_show_research_toolbar->setEnabled(true);
    UpdateData(true);
}
