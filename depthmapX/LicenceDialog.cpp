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

#include "LicenceDialog.h"

const char *g_agreement = "see http://www.gnu.org/licenses/";

CLicenceDialog::CLicenceDialog(QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	m_message = tr("");
	m_agreement = tr("");
}

void CLicenceDialog::OnOK()
{
	UpdateData(true);
	accept();
}

void CLicenceDialog::OnCancel()
{
	reject();
}

void CLicenceDialog::UpdateData(bool value)
{
	if (value)
	{
		m_message = c_message->text();
		m_agreement = c_agreement->toPlainText();
	}
	else
	{
		c_message->setText(m_message);
		c_agreement->setPlainText(m_agreement);
	}
}

void CLicenceDialog::showEvent(QShowEvent * event)
{
	setWindowTitle(m_title);

	m_message =
        tr("old");
	m_agreement = 
        QString("old") + tr("old") +
        QString("old") + g_agreement;

	UpdateData(false);
}
