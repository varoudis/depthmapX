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

#include "MakeLayerDlg.h"

CMakeLayerDlg::CMakeLayerDlg(QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	m_keeporiginal = true;
	m_remove_stubs = false;
	m_push_values = false;
	m_percentage = 0;
	m_origin = tr("");
	m_layer_name = tr("");
	m_mapin = 0;
	m_mapout = 0;
}

void CMakeLayerDlg::OnRemoveStubs(bool value)
{
	if (value) {
        c_percentage->setText(tr("40")); // Bill: 40% (old 25)
		c_percentage->setEnabled(true);
	}
	else {
		c_percentage->setText(tr("0"));
		c_percentage->setEnabled(false);
	}
}

void CMakeLayerDlg::OnOK()
{
	UpdateData(true);
	m_mapout = m_lookup[c_layer_type->currentIndex()];
	accept();
}

void CMakeLayerDlg::OnCancel()
{
	reject();
}

void CMakeLayerDlg::UpdateData(bool value)
{
	if (value)
	{
		if (c_remove_stubs->checkState())
			m_remove_stubs = true;
		else
			m_remove_stubs = false;
		
		if (c_push_values->checkState())
			m_push_values = true;
		else
			m_push_values = false;
		m_percentage = c_percentage->text().toInt();
		m_origin = c_origin->text();
		m_layer_name = c_layer_name->text();

		if (c_keeporiginal->checkState())
			m_keeporiginal = true;
		else
			m_keeporiginal = false;
	}
	else
	{
		if (m_remove_stubs)
			c_remove_stubs->setCheckState(Qt::Checked);
		else
			c_remove_stubs->setCheckState(Qt::Unchecked);
		
		if (m_push_values)
			c_push_values->setCheckState(Qt::Checked);
		else
			c_push_values->setCheckState(Qt::Unchecked);
		
		c_percentage->setText(QString(tr("%1")).arg(m_percentage));
		c_origin->setText(m_origin);
		c_layer_name->setText(m_layer_name);

		if (m_keeporiginal)
			c_keeporiginal->setCheckState(Qt::Checked);
		else
			c_keeporiginal->setCheckState(Qt::Unchecked);
	}
}

static int item_process = 0;
void CMakeLayerDlg::showEvent(QShowEvent * event)
{
	item_process = 1;
	if (m_mapout & MAKELAYER_DRAWING) {
		c_layer_type->addItem(QString(tr("Drawing Map")));
		m_lookup.push_back(MAKELAYER_DRAWING);
	}
	if (m_mapout & MAKELAYER_DATA) {
		c_layer_type->addItem(QString(tr("Data Map")));
		m_lookup.push_back(MAKELAYER_DATA);
	}
	if (m_mapout & MAKELAYER_AXIAL) {
		c_layer_type->addItem(QString(tr("Axial Map")));
		m_lookup.push_back(MAKELAYER_AXIAL);
	}
	if (m_mapout & MAKELAYER_CONVEX) {
		c_layer_type->addItem(QString(tr("Convex Map")));
		m_lookup.push_back(MAKELAYER_CONVEX);
	}
	if (m_mapout & MAKELAYER_SEGMENT) {
		c_layer_type->addItem(QString(tr("Segment Map")));
		m_lookup.push_back(MAKELAYER_SEGMENT);
	}

	c_layer_type->setCurrentIndex(0);

	if (m_mapin == MAKELAYER_DRAWING) {
		// hide push values:
		c_push_values->hide();
		// hide retain map:
		c_keeporiginal->hide();
		// make the dialog a bit smaller...
		QRect winrect;
		winrect = geometry();
		winrect.setBottom(winrect.bottom() - 75);
		setGeometry(winrect);
		winrect = c_ok->geometry();
		winrect.setTop(winrect.top() - 75);
		winrect.setBottom(winrect.bottom() - 75);
		c_ok->setGeometry(winrect);
		//wnd = GetDlgItem(IDCANCEL);
		winrect = c_cancel->geometry();
		winrect.setTop(winrect.top() - 75);
		winrect.setBottom(winrect.bottom() - 75);
		c_cancel->setGeometry(winrect);
	}
	if (m_mapin != MAKELAYER_AXIAL) {
		// hide remove stubs:
		c_remove_stubs->hide();
		c_percentage->hide();
		label_4->hide();
	}

	c_layer_type->setCurrentIndex(0);

	UpdateData(false);
	item_process = 0;
    OnSelchangeLayerType(0);
}

void CMakeLayerDlg::OnSelchangeLayerType(int value)
{
	if(item_process) return;
	int which = m_lookup[value];

	switch (which) {
	case MAKELAYER_DRAWING:
	   c_layer_name->setText(tr("Drawing Map"));
	   break;
	case MAKELAYER_DATA:
	   c_layer_name->setText(tr("Gate Map"));
	   break;
	case MAKELAYER_AXIAL:
	   c_layer_name->setText(tr("Axial Map"));
	   break;
	case MAKELAYER_CONVEX:
	   c_layer_name->setText(tr("Convex Map"));//GetDlgItem(IDC_LAYER_NAME)->SetWindowText(_T("Convex Map"));
	   break;
	case MAKELAYER_SEGMENT:
	   c_layer_name->setText(tr("Segment Map"));
	   break;
	}

	if (which == MAKELAYER_SEGMENT) {
		c_keeporiginal->setEnabled(true);
		c_push_values->setEnabled(true);
		c_remove_stubs->setEnabled(true);
		c_percentage->setEnabled(true);
		label_4->setEnabled(true);
	}
	else if (which == MAKELAYER_DRAWING) {
		c_keeporiginal->setEnabled(false);
		c_keeporiginal->setCheckState(Qt::Checked);
		c_push_values->setEnabled(false);
		c_push_values->setCheckState(Qt::Unchecked);
		c_remove_stubs->setEnabled(false);
		c_remove_stubs->setCheckState(Qt::Unchecked);
		c_percentage->setEnabled(false);
		c_percentage->setText(tr("0"));//GetDlgItem(IDC_PERCENTAGE_LENGTH)->SetWindowText(_T("0"));
		label_4->setEnabled(false);
	}
	else {
		c_keeporiginal->setEnabled(true);
		c_push_values->setEnabled(true);
		c_remove_stubs->setEnabled(false);
		c_percentage->setEnabled(false);
		label_4->setEnabled(false);
	}
}
