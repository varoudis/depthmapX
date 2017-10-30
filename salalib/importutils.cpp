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

    const int DXFCIRCLERES = 36;

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

            QtRegion region;

            for(auto& point: points) {
                region = runion(region, point);
            }

            mgraph.importPointsAsShapeMap(points, region, name, table);

        } else if (x1col != -1 && y1col != -1 && x2col != -1 && y2col != -1) {
            std::vector<Line> lines = extractLines(table[columns[x1col]], table[columns[y1col]], table[columns[x2col]], table[columns[y2col]]);
            table.erase(table.find(columns[x1col]));
            table.erase(table.find(columns[y1col]));
            table.erase(table.find(columns[x2col]));
            table.erase(table.find(columns[y2col]));

            QtRegion region;

            for(auto& line: lines) {
                region = runion(region, line);
            }

            //mgraph.importLinesAsShapeMap(lines, region, name, table);
            int shapeMapIndex = mgraph.createNewDrawingLayer(name);
            mgraph.getDrawingLayer(shapeMapIndex).init(lines.size(),region);
            mgraph.getDrawingLayer(shapeMapIndex).importLines(lines, table);
            mgraph.initDrawingLayer(shapeMapIndex);
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

    int importDxf(MetaGraph &mgraph, std::istream &stream, Communicator *communicator)
    {
        DxfParser dp;

        if (communicator) {
            dp = DxfParser( communicator );

            try {
                *communicator >> dp;
            }
            catch (Communicator::CancelledException) {
                return 0;
            }
            catch (pexception) {
                return -1;
            }

            if (communicator->IsCancelled()) {
                return 0;
            }
        }
        else {
            dp.open(stream);
        }

        int i = 0;

        mgraph.SuperSpacePixel::tail().m_region = QtRegion(dp.getExtMin(), dp.getExtMax());

        for (auto& layer: dp.getLayers())
        {
            const DxfLayer& dxf_layer = layer.second;

            if (dxf_layer.empty()) {
                continue;
            }

            std::vector<Point2f> points;
            std::vector<Line> lines;
            std::vector<Polyline> polylines;

            for (int jp = 0; jp < dxf_layer.numPoints(); jp++) {
                const DxfVertex& dxf_point = dxf_layer.getPoint( jp );
                points.push_back(Point2f(dxf_point));

            }

            for (int j = 0; j < dxf_layer.numLines(); j++) {
                const DxfLine& dxf_line = dxf_layer.getLine( j );
                lines.push_back(Line( Point2f(dxf_line.getStart()), Point2f(dxf_line.getEnd()) ));
            }

            for (int k = 0; k < dxf_layer.numPolyLines(); k++) {

                const DxfPolyLine& poly = dxf_layer.getPolyLine( k );
                std::vector<Point2f> points;
                for (int m = 0; m < poly.numVertices(); m++) {
                    points.push_back(poly.getVertex(m));
                }
                polylines.push_back(depthmapX::Polyline(points, (poly.getAttributes() & DxfPolyLine::CLOSED) == DxfPolyLine::CLOSED));
            }

            for (int l = 0; l < dxf_layer.numSplines(); l++) {

                const DxfSpline& poly = dxf_layer.getSpline( l );

                std::vector<Point2f> points;
                for (int m = 0; m < poly.numVertices(); m++) {
                    points.push_back(poly.getVertex(m));
                }
                polylines.push_back(depthmapX::Polyline(points, (poly.getAttributes() & DxfPolyLine::CLOSED) == DxfPolyLine::CLOSED));

            }

            for (int n = 0; n < dxf_layer.numArcs(); n++) {
                const DxfArc& circ = dxf_layer.getArc( n );
                int segments = circ.numSegments(DXFCIRCLERES);
                if (segments > 1) {
                    for (int m = 0; m <= segments; m++) {
                        points.push_back(circ.getVertex(m, segments));
                    }
                }
                polylines.push_back(depthmapX::Polyline(points, false));
            }

            for (int nc = 0; nc < dxf_layer.numCircles(); nc++) {
                const DxfCircle& circ = dxf_layer.getCircle( nc );
                std::vector<Point2f> points;
                for (int m = 0; m < DXFCIRCLERES; m++) {
                    points.push_back(circ.getVertex(m,DXFCIRCLERES));
                }
                polylines.push_back(depthmapX::Polyline(points, true));
            }

            QtRegion region = QtRegion(Point2f(dxf_layer.getExtMin()),Point2f(dxf_layer.getExtMax()));

            int shapeMapIndex = mgraph.createNewDrawingLayer(dxf_layer.getName());
            mgraph.getDrawingLayer(shapeMapIndex).init(points.size() + lines.size() + polylines.size(),region);
            // parameters could be passed in the Table here such as the layer/block/colour/linetype etc.
            mgraph.getDrawingLayer(shapeMapIndex).importPoints(points, Table());
            mgraph.getDrawingLayer(shapeMapIndex).importLines(lines, Table());
            mgraph.getDrawingLayer(shapeMapIndex).importPolylines(polylines, Table());
            // mgraph.initDrawingLayer(shapeMapIndex);

            i++;
        }

       return 1;
    }
}
