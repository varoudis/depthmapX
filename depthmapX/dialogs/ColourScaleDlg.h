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

#ifndef ColourScaleDlg_H
#define ColourScaleDlg_H

#include "ui_ColourScaleDlg.h"
#include "GraphDoc.h"

class CColourScaleDlg : public QDialog, public Ui::CColourScaleDlg
{
	Q_OBJECT
public:
	CColourScaleDlg(QWidget *parent = 0);
	QGraphDoc *m_viewDoc;
	QString	m_blue;
	QString	m_red;
	int		m_color;

	bool m_docked;

	double m_display_min;
	double m_display_max;
	double GetActualValue(double sliderpos);
	float GetNormValue(double actualval);

	QBrush m_red_brush;
	QBrush m_blue_brush;
	DisplayParams m_displayparams;

	void MyUpdateData(bool dir, bool apply_to_all);
	void Clear();
	void Fill();
	bool m_show_lines;
	bool m_show_fill;
	bool m_show_centroids;
    std::vector<int> m_color_type_map;
	void UpdateData(bool value);
	void showEvent(QShowEvent * event);
	void OnFocusGraph(QGraphDoc* pDoc, int lParam);

private slots:
		void OnChangeBlueValue();
		void OnChangeRedValue();
		void OnReleasedRedSlider(int);
		void OnReleasedBlueSlider(int);
		void OnSelchangeColor(int);
		void OnBnClickedShowLines(bool);
		void OnBnClickedShowFill(bool);
		void OnBnClickedShowCentroids(bool);
		void OnBnClickedApplytoall();
		void OnOK();
		void OnCancel();
};

#endif
