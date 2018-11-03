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

#include "AttributeSummary.h"
#include "GraphDoc.h"
#include "ColumnPropertiesDlg.h"

CAttributeSummary::CAttributeSummary(QGraphDoc *pDoc, QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	m_pDoc = pDoc;
}

void CAttributeSummary::OnOK()
{
	accept();
}

void CAttributeSummary::OnDblclkList(int row, int column)
{
	if (row != -1) {
		CColumnPropertiesDlg dlg(&(m_pDoc->m_meta_graph->getAttributeTable()), row);
		dlg.exec();
	}
}

void CAttributeSummary::UpdateData(bool value)
{

}

void CAttributeSummary::showEvent(QShowEvent * event)
{
	const AttributeTable& table = m_pDoc->m_meta_graph->getAttributeTable();
	
	c_list->setSelectionBehavior(QAbstractItemView::SelectRows);

    c_list->setColumnCount(4);
	
	QTableWidgetItem *Item;
	Item = new QTableWidgetItem("Attribute");
	c_list->setColumnWidth(0, 220);
	Item->setTextAlignment(Qt::AlignLeft);
	c_list->setHorizontalHeaderItem(0, Item);

	Item = new QTableWidgetItem("Minimum");
	c_list->setColumnWidth(1, 100);
	Item->setTextAlignment(Qt::AlignRight);
	c_list->setHorizontalHeaderItem(1, Item);

	Item = new QTableWidgetItem("Average");
	c_list->setColumnWidth(2, 100);
	Item->setTextAlignment(Qt::AlignRight);
	c_list->setHorizontalHeaderItem(2, Item);

	Item = new QTableWidgetItem("Maximum");
	c_list->setColumnWidth(3, 100);
	Item->setTextAlignment(Qt::AlignRight);
	c_list->setHorizontalHeaderItem(3, Item);
	
	c_list->clearContents();

	c_list->setRowCount(table.getColumnCount());
	for (int i = 0; i < table.getColumnCount(); i++) {
		Item = new QTableWidgetItem(QString(table.getColumnName(i).c_str()));
		Item->setFlags(Qt::NoItemFlags);
		c_list->setRowHeight(i, 20);
		c_list->setItem(i, 0, Item);
		//
		char text[64];
		// Min
		sprintf(text, "%g", table.getMinValue(i));
		Item = new QTableWidgetItem(QString(text));
		Item->setFlags(Qt::NoItemFlags);
		c_list->setItem(i, 1, Item);
		// Avg
		sprintf(text,"%g",table.getAvgValue(i));
		Item = new QTableWidgetItem(QString(text));
		Item->setFlags(Qt::NoItemFlags);
		c_list->setItem(i, 2, Item);
		// Max
		sprintf(text,"%g",table.getMaxValue(i));
		Item = new QTableWidgetItem(QString(text));
		Item->setFlags(Qt::NoItemFlags);
		c_list->setItem(i, 3, Item);
	}
}
