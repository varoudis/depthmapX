// Copyright (C) 2011-2012, Tasos Varoudis
// Copyright (C) 2018, Petros Koutsolampros

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

#include "AxialAnalysisOptionsDlg.h"
#include "mainwindow.h"
#include <QMessageBox>

CAxialAnalysisOptionsDlg::CAxialAnalysisOptionsDlg(MetaGraph *graph, QWidget *parent) : QDialog(parent) {
    setupUi(this);
    m_radius = QString(tr(""));
    m_choice = false;
    m_attribute = -1;
    m_weighted = false;
    m_rra = false;
    m_local = false;

    m_meta_graph = graph;

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin) {
            m_choice = mainWin->m_options.choice;
            m_local = mainWin->m_options.local;
            m_rra = mainWin->m_options.fulloutput;

            m_radius = QString(tr("n"));

            if (mainWin->m_options.weighted_measure_col == -1) {
                m_weighted = false;
                m_attribute = -1;
            } else {
                m_weighted = true;
                m_attribute = 0;
            }
            break;
        }
    }

    if (m_choice)
        c_choice->setCheckState(Qt::Checked);
    else
        c_choice->setCheckState(Qt::Unchecked);

    if (m_local)
        c_local->setCheckState(Qt::Checked);
    else
        c_local->setCheckState(Qt::Unchecked);

    if (m_rra)
        c_rra->setCheckState(Qt::Checked);
    else
        c_rra->setCheckState(Qt::Unchecked);

    if (m_weighted)
        c_weighted->setCheckState(Qt::Checked);
    else
        c_weighted->setCheckState(Qt::Unchecked);

    c_attribute_chooser->setCurrentIndex(m_attribute);
    c_radius->setText(m_radius);
}

void CAxialAnalysisOptionsDlg::OnUpdateRadius() {
    QString text;
    text = c_radius->text();
    if (!text.isEmpty() && text.indexOf("n") == -1 && text.indexOf("N") == -1 && text.indexOf("1") == -1 &&
        text.indexOf("2") == -1 && text.indexOf("3") == -1 && text.indexOf("4") == -1 && text.indexOf("5") == -1 &&
        text.indexOf("6") == -1 && text.indexOf("7") == -1 && text.indexOf("8") == -1 && text.indexOf("9") == -1) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("The radius must either be numeric or 'n'\n Alternatively, for multiple radii, type a "
                                "list of comma separated numeric radii (you can include 'n')"),
                             QMessageBox::Ok, QMessageBox::Ok);
        c_radius->setText(tr("n"));
        c_radius->setFocus(Qt::OtherFocusReason);
    }
}

void CAxialAnalysisOptionsDlg::OnWeighted() {
    if (c_weighted->checkState()) {
        UpdateData(true);
        c_attribute_chooser->setEnabled(true);
        m_attribute = 0;
        UpdateData(false);
    } else {
        UpdateData(true);
        c_attribute_chooser->setEnabled(false);
        m_attribute = -1;
        UpdateData(false);
    }
}

void CAxialAnalysisOptionsDlg::OnOK() {
    UpdateData(true);

    if (m_radius.isEmpty() ||
        (m_radius.indexOf("n") == -1 && m_radius.indexOf("N") == -1 && m_radius.indexOf("1") == -1 &&
         m_radius.indexOf("2") == -1 && m_radius.indexOf("3") == -1 && m_radius.indexOf("4") == -1 &&
         m_radius.indexOf("5") == -1 && m_radius.indexOf("6") == -1 && m_radius.indexOf("7") == -1 &&
         m_radius.indexOf("8") == -1 && m_radius.indexOf("9") == -1)) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("The radius must either be numeric or 'n'\n Alternatively, for multiple radii, type a "
                                "list of comma separated numeric radii (you can include 'n')"),
                             QMessageBox::Ok, QMessageBox::Ok);
        m_radius = tr("n");
        UpdateData(false);
        c_radius->setFocus(Qt::OtherFocusReason);
        return;
    }
    // now parse radius list:
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin) {
            mainWin->m_options.radius_set.clear();
            QString curr_radius;
            int curr_comma = -1, last_comma = 0;
            bool add_rn = false;
            do {
                curr_comma = m_radius.indexOf(',', last_comma);
                if (curr_comma != -1) {
                    curr_radius = m_radius.mid(last_comma, curr_comma - last_comma);
                    last_comma = curr_comma + 1;
                } else {
                    curr_radius = m_radius.mid(last_comma);
                }
                if (!curr_radius.isEmpty()) {
                    if (curr_radius == "n" || curr_radius == "N") {
                        add_rn = true;
                    } else {
                        int radius = curr_radius.toInt();
                        if (radius <= 0) {
                            QMessageBox::warning(
                                this, tr("Warning"),
                                tr("Each radius in the list must either be 'n' or a number in the range 1-99"),
                                QMessageBox::Ok, QMessageBox::Ok);
                            c_radius->setFocus(Qt::OtherFocusReason);
                            return;
                        }
                        mainWin->m_options.radius_set.insert(static_cast<double>(radius));
                    }
                }
            } while (curr_comma != -1);
            if (mainWin->m_options.radius_set.size() == 0 || add_rn) {
                mainWin->m_options.radius_set.insert(-1);
            }

            mainWin->m_options.choice = m_choice;
            mainWin->m_options.local = m_local;
            mainWin->m_options.fulloutput = m_rra;

            // attributes:
            if (!m_weighted) {
                mainWin->m_options.weighted_measure_col = -1;
            } else {
                mainWin->m_options.weighted_measure_col = m_attribute;
            }
            break;
        }
    }

    accept();
}

void CAxialAnalysisOptionsDlg::UpdateData(bool value) {
    if (value) {
        m_radius = c_radius->text();
        if (c_choice->checkState())
            m_choice = true;
        else
            m_choice = false;

        m_attribute = c_attribute_chooser->currentIndex();

        if (c_weighted->checkState())
            m_weighted = true;
        else
            m_weighted = false;

        if (c_rra->checkState())
            m_rra = true;
        else
            m_rra = false;

        if (c_local->checkState())
            m_local = true;
        else
            m_local = false;
    } else {
        c_radius->setText(m_radius);
        if (m_choice)
            c_choice->setCheckState(Qt::Checked);
        else
            c_choice->setCheckState(Qt::Unchecked);

        c_attribute_chooser->setCurrentIndex(m_attribute);
        if (m_weighted)
            c_weighted->setCheckState(Qt::Checked);
        else
            c_weighted->setCheckState(Qt::Unchecked);

        if (m_rra)
            c_rra->setCheckState(Qt::Checked);
        else
            c_rra->setCheckState(Qt::Unchecked);

        if (m_local)
            c_local->setCheckState(Qt::Checked);
        else
            c_local->setCheckState(Qt::Unchecked);
    }
}

void CAxialAnalysisOptionsDlg::showEvent(QShowEvent *event) {
    const ShapeGraph &map = m_meta_graph->getDisplayedShapeGraph();
    const AttributeTable &table = map.getAttributeTable();
    for (int i = 0; i < table.getNumColumns(); i++) {
        c_attribute_chooser->addItem(QString(table.getColumnName(i).c_str()));
    }

    if (m_weighted) {
        c_attribute_chooser->setEnabled(true);
        m_attribute = 0;
        c_attribute_chooser->setCurrentIndex(m_attribute);
    } else {
        m_attribute = -1;
        c_attribute_chooser->setCurrentIndex(m_attribute);
        c_attribute_chooser->setEnabled(true);
    }

    // UpdateData(false);
}
