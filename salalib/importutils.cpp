// Copyright (C) 2017 Petros Koutsolampros

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

#include "salalib/importutils.h"
#include "genlib/stringutils.h"
#include <sstream>

namespace depthmapX {

    bool importTxt(MetaGraph &mgraph, std::istream &stream, std::string name, char delimiter = '\t') {
        Table table = csvToTable(stream, delimiter);
        std::vector<std::string> columns;
        int xcol = -1, ycol = -1, x1col = -1, y1col = -1, x2col = -1, y2col = -1;
        for(auto const& column: table) {
            if (column.first == "x" || column.first == "easting")
                xcol = columns.size();
            else if (column.first == "y" || column.first == "northing")
                ycol = columns.size();
            else if (column.first == "x1")
                x1col = columns.size();
            else if (column.first == "x2")
                x2col = columns.size();
            else if (column.first == "y1")
                y1col = columns.size();
            else if (column.first == "y2")
                y2col = columns.size();
            columns.push_back(column.first);
        }

        if (xcol != -1 && ycol != -1) {
            std::vector<Point2f> points = extractPoints(table[columns[xcol]], table[columns[ycol]]);
            table.erase(table.find(columns[xcol]));
            table.erase(table.find(columns[ycol]));

            mgraph.importPointsAsShapeMap(points, name, table);

        } else if (x1col != -1 && y1col != -1 && x2col != -1 && y2col != -1) {
            std::vector<Line> lines = extractLines(table[columns[x1col]], table[columns[y1col]], table[columns[x2col]], table[columns[y2col]]);
            table.erase(table.find(columns[x1col]));
            table.erase(table.find(columns[y1col]));
            table.erase(table.find(columns[x2col]));
            table.erase(table.find(columns[y2col]));

            //mgraph.importLinesAsShapeMap(lines, name, table);
            mgraph.importLinesAsDrawingLayer(lines, name, table);
        }
        return true;
    }

    Table csvToTable(std::istream &stream, char delimiter = '\t') {

        Table table;
        std::vector<std::string> columns;

        std::string inputline;
        std::getline(stream, inputline);

        // check for a matching delimited header line...
        auto strings = dXstring::split(inputline, delimiter);
        if (strings.size() < 2) {
            // throw exception
            return table;
        }

        for (auto& columnName: strings) {
            if (!columnName.empty()) {
                dXstring::ltrim(columnName,'\"');
                dXstring::rtrim(columnName,'\"');
            }
            table.insert( std::make_pair( columnName, std::vector<std::string>() ) );
            columns.push_back( columnName );
        }

        while (!stream.eof()) {
            std::getline(stream, inputline);
            if (!inputline.empty()) {
                auto strings = dXstring::split(inputline, delimiter);
                if(strings.size() != columns.size()) {
                    std::stringstream message;
                    message << "Cells in line " << inputline << "not the same number as the columns" << flush;
                    throw RuntimeException(message.str().c_str());
                }
                if (!strings.size()) {
                    continue;
                }
                for (size_t i = 0; i < strings.size(); i++) {
                    table[columns[i]].push_back(strings[i]);
                }
            }
        }
        return table;
    }

    std::vector<Line> extractLines(ColumnData &x1col, ColumnData &y1col, ColumnData &x2col, ColumnData &y2col) {
        std::vector<Line> lines;
        for(size_t i = 0; i < x1col.size(); i++) {
            double x1 = stod(x1col[i]);
            double y1 = stod(y1col[i]);
            double x2 = stod(x2col[i]);
            double y2 = stod(y2col[i]);
            lines.push_back(Line(Point2f(x1, y1), Point2f(x2, y2)));
        }
        return lines;
    }
    std::vector<Point2f> extractPoints(ColumnData &x, ColumnData &y) {
        std::vector<Point2f> points;
        for(size_t i = 0; i < x.size(); i++) {
            points.push_back(Point2f(stod(x[i]), stod(y[i])));
        }
        return points;
    }
}
