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

#include "PromptReplace.h"

CPromptReplace::CPromptReplace(QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	m_message = tr("");

	UpdateData(false);
}

void CPromptReplace::OnAdd()
{
	UpdateData(true);
	done(1);
}

void CPromptReplace::OnReplace()
{
	UpdateData(true);
	done(2);
}

void CPromptReplace::OnCancel()
{
	reject();
}

void CPromptReplace::UpdateData(bool value)
{
	if (value)
		m_message = c_message->text();
	else
		c_message->setText(m_message);
}

void CPromptReplace::showEvent(QShowEvent * event)
{
	UpdateData(false);
}
