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

#include "ui_MakeLayerDlg.h"
#include <salalib/mgraph.h>
#include <salalib/attributes.h>
#include <salalib/shapemap.h>
#include <salalib/axialmap.h>

enum {MAKELAYER_DRAWING = 1, 
MAKELAYER_DATA = 2,
MAKELAYER_AXIAL = 4,
MAKELAYER_CONVEX = 8, 
MAKELAYER_GENERIC = 16,
MAKELAYER_SEGMENT = 32
};

class CMakeLayerDlg : public QDialog, public Ui::CMakeLayerDlg
{
	Q_OBJECT
public:
	CMakeLayerDlg(QWidget *parent = 0);
	bool	m_remove_stubs;
	bool	m_push_values;
	int		m_percentage;
	QString	m_origin;
	QString	m_layer_name;

	int m_mapin;
	int m_mapout;

	pvecint m_lookup;
	bool m_keeporiginal;
	void UpdateData(bool value);
	void showEvent(QShowEvent * event);

private slots:
		void OnSelchangeLayerType(int);
		void OnRemoveStubs(bool);
		void OnOK();
		void OnCancel();
};
