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


#include "ui_SegmentAnalysisDlg.h"
#include <salalib/mgraph.h>
#include <salalib/attributes.h>
#include <salalib/shapemap.h>
#include <salalib/axialmap.h>

class CSegmentAnalysisDlg : public QDialog, public Ui::CSegmentAnalysisDlg
{
	Q_OBJECT
public:
	CSegmentAnalysisDlg(MetaGraph *graph = NULL, QWidget *parent = 0);
	int		m_analysis_type;
	QString	m_radius;
	int		m_tulip_bins;
	int		m_radius_type;
	bool	m_choice;
	bool	m_weighted;
	int		m_attribute;
	void UpdateData(bool value);
	void showEvent(QShowEvent * event);

	MetaGraph *m_meta_graph;

private slots:
	void OnAnalysisType(bool);
	void OnAnalysisTulip(bool);
	void OnUpdateRadius(QString);
	void OnWeighted(bool);
	void OnOK();
	void OnCancel();
};
