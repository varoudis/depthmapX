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

#include "AgentAnalysisDlg.h"

CAgentAnalysisDlg::CAgentAnalysisDlg(QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	m_release_location = -1;
	m_fov = 0;
	m_frames = 0;
	m_release_rate = 0.0;
	m_steps = 0;
	m_timesteps = 0;
	m_occlusion = -1;
	m_record_trails = false;
	m_trail_count = 0;
	
	m_trail_count = 50;
	m_occlusion = 0;
	m_gatelayer = -1;
	m_release_location = 0;
}

void CAgentAnalysisDlg::OnOK()
{
	m_timesteps = c_timesteps->text().toInt();
	m_release_location = c_release_location->isChecked();
	m_release_rate = c_release_rate->text().toDouble();
	m_fov = c_fov->text().toInt();
	m_steps = c_steps->text().toInt();
	m_frames = c_frames->text().toInt();
	if (c_record_trails->checkState())
	{
		m_record_trails = true;
	}
	else
		m_record_trails = false;
	m_occlusion = c_occlusion->currentIndex();
	m_trail_count = c_trail_count->text().toInt();
	m_gatelayer = c_layer_selector->currentIndex() - 1;
	accept();
}

void CAgentAnalysisDlg::OnCancel()
{
	reject();
}

void CAgentAnalysisDlg::UpdateData(bool value)
{
	if (value)
	{
	}
	else
	{

	}
}

void CAgentAnalysisDlg::showEvent(QShowEvent * event)
{
	c_timesteps->setText(QString("%1").arg(m_timesteps));
	c_release_location->setChecked(m_release_location);
	c_release_rate->setText(QString("%1").arg(m_release_rate));
	c_fov->setText(QString("%1").arg(m_fov));
	c_steps->setText(QString("%1").arg(m_steps));
	c_frames->setText(QString("%1").arg(m_frames));
	if (m_record_trails)
	{
		c_record_trails->setCheckState(Qt::Checked);
	}
	else
		c_record_trails->setCheckState(Qt::Unchecked);
	c_occlusion->setCurrentIndex(m_occlusion);
	c_trail_count->setText(QString("%1").arg(m_trail_count));
	for (size_t i = 0; i < m_names.size(); i++) {
		c_layer_selector->addItem( QString(m_names[i].c_str()) );
	}
	c_layer_selector->setCurrentIndex(m_gatelayer + 1);
}
