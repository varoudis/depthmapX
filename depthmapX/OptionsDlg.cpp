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

#include "OptionsDlg.h"
#include "mainwindow.h"
#include <QMessageBox>

COptionsDlg::COptionsDlg(QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	m_global = false;
	m_local = false;
	m_radius = tr("");
	m_gates_only = false;
	m_output_type = -1;
	m_radius2 = tr("");

	foreach (QWidget *widget, QApplication::topLevelWidgets()) {
		MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
		if (mainWin)
		{
			m_output_type = mainWin->m_options.output_type;

			m_local   = mainWin->m_options.local;
			m_global  = mainWin->m_options.global;
			m_gates_only = mainWin->m_options.gates_only;

			if ((int) mainWin->m_options.radius == -1) {
				m_radius = QString("n");
				m_radius2 = QString("n");
			}
			else if (m_output_type == Options::OUTPUT_VISUAL) {
				char number[2];
				sprintf( number, "%d", (int) mainWin->m_options.radius );
				m_radius = QString(number);
				m_radius2 = tr("n");
			}
			else {
				char number[32];
				sprintf( number, "%g", mainWin->m_options.radius );
				m_radius = tr("n");
				m_radius2 = QString(number);
			}
			break;
		}
	}
}

void COptionsDlg::OnOutputType(bool value)
{
	UpdateData(true);

	if (m_output_type == Options::OUTPUT_VISUAL) {
		c_local->setEnabled(true);
		c_global->setEnabled(true);
		c_radius->setEnabled(true);
	}
	else {
		c_local->setEnabled(false);
		c_global->setEnabled(false);
		c_radius->setEnabled(false);
		c_radius->setText(tr("n"));// <- essentially, undo changes
	}

	if (m_output_type == Options::OUTPUT_METRIC) {
		c_radius2->setEnabled(true);
	}
	else {
		c_radius2->setText(tr("n"));// <- essentially, undo changes
		c_radius2->setEnabled(false);
	}
}

void COptionsDlg::OnUpdateRadius(QString text)
{
	if (text.length()) {
		if (!text.toInt() && text != tr("n")) {
			QMessageBox::warning(this, tr("Warning"), tr("The radius must either be n or number in range 1-99"), QMessageBox::Ok, QMessageBox::Ok);
			c_radius->setText(tr("n"));
		}
	}		
}

void COptionsDlg::OnUpdateRadius2(QString text)
{
	if (text.length()) {
		if (text.toDouble() == 0.0 && text != tr("n")) {
			QMessageBox::warning(this, tr("Warning"), tr("The radius must either be n or a positive number"), QMessageBox::Ok, QMessageBox::Ok);
			c_radius2->setText(tr("n"));
		}
	}		
}

void COptionsDlg::OnOK()
{
	UpdateData(true);

	foreach (QWidget *widget, QApplication::topLevelWidgets()) {
		MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
		if (mainWin)
		{
			mainWin->m_options.local  = m_local;
			mainWin->m_options.global = m_global;
			mainWin->m_options.output_type = m_output_type;
			mainWin->m_options.gates_only = m_gates_only;
			mainWin->m_options.gatelayer = c_layer_selector->currentIndex() - 1;

			if (m_output_type == Options::OUTPUT_VISUAL) {
				if (m_radius.compare(tr("n")) == 0) { // 0 means identical
					mainWin->m_options.radius = -1.0;
				}
				else {
					mainWin->m_options.radius = (double) m_radius.toInt();
					if (mainWin->m_options.radius <= 0.0) {
						QMessageBox::warning(this, tr("Warning"), tr("The radius must either be n or a number in the range 1-99"), QMessageBox::Ok, QMessageBox::Ok);
						return;
					}
				}
			}
			else {
				if (m_radius2.compare(tr("n")) == 0) { // 0 means identical
					mainWin->m_options.radius = -1.0;
				}
				else {
					mainWin->m_options.radius = m_radius2.toDouble();
					if (mainWin->m_options.radius <= 0.0) {
						QMessageBox::warning(this, tr("Warning"), tr("The radius must either be n or a positive number"), QMessageBox::Ok, QMessageBox::Ok);
						return;
					}
				}
			}
			break;
		}
	}
	accept();
}

void COptionsDlg::OnCancel()
{
	reject();
}

void COptionsDlg::UpdateData(bool value)
{
	if (value)
	{
		if (c_global->checkState())
			m_global = true;
		else
			m_global = false;

		if (c_local->checkState())
			m_local = true;
		else
			m_local = false;

		m_radius = c_radius->text();
		
		if (c_output_type->isChecked())
			m_output_type = 0;
		else if (c_radio1->isChecked())
			m_output_type = 1;
		else if (c_radio2->isChecked())
			m_output_type = 2;
		else if (c_radio3->isChecked())
			m_output_type = 3;
		else if (c_radio4->isChecked())
			m_output_type = 4;
		else
			m_output_type = -1;
		m_radius2 = c_radius2->text();
	}
	else
	{
		if (m_global)
			c_global->setCheckState(Qt::Checked);
		else
			c_global->setCheckState(Qt::Unchecked);

		if (m_local)
			c_local->setCheckState(Qt::Checked);
		else
			c_local->setCheckState(Qt::Unchecked);

		c_radius->setText(m_radius);

		switch (m_output_type)
		{
		case 0:
			c_output_type->setChecked(true);
			break;
		case 1:
			c_radio1->setChecked(true);
			break;
		case 2:
			c_radio2->setChecked(true);
			break;
		case 3:
			c_radio3->setChecked(true);
			break;
		case 4:
			c_radio4->setChecked(true);
			break;
		default:
			break;
		}
		c_radius2->setText(m_radius2);
	}
}

void COptionsDlg::showEvent(QShowEvent * event)
{
	for (size_t i = 0; i < m_layer_names.size(); i++) {
		c_layer_selector->addItem(QString(m_layer_names[i].c_str()));
	}
	foreach (QWidget *widget, QApplication::topLevelWidgets()) {
		MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
		if (mainWin)
		{
			c_layer_selector->setCurrentIndex(mainWin->m_options.gatelayer + 1);
			break;
		}
	}

	OnOutputType(false);

	UpdateData(false);
}
