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

#include "AttributeChooserDlg.h"

CAttributeChooserDlg::CAttributeChooserDlg(AttributeTable& table, QWidget *parent)
: QDialog(parent)
{
	setupUi(this);
	c_attribute_chooser->setCurrentIndex(-1);//m_attribute = -1;
	c_text->setText(QString(tr("")));
	m_table = &table;
}

void CAttributeChooserDlg::OnOK()
{
	m_attribute = c_attribute_chooser->currentIndex();
	m_text = c_text->text();
	m_attribute--;
	accept();
}

void CAttributeChooserDlg::OnCancel()
{
	reject();
}

void CAttributeChooserDlg::UpdateData(bool value)
{

}

void CAttributeChooserDlg::showEvent(QShowEvent * event)
{
	c_attribute_chooser->addItem(QString(tr("Ref Number")));
    for (int i = 0; i < m_table->getNumColumns(); i++) {
		c_attribute_chooser->addItem( QString(m_table->getColumnName(i).c_str()) );
	}
	c_attribute_chooser->setCurrentIndex(0);
}
