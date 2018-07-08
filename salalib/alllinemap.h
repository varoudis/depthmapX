#pragma once

#include "salalib/axialmap.h"
#include "salalib/axialpolygons.h"

class AllLineMap: public ShapeGraph
{
public:
    AllLineMap(Communicator *comm,
               std::vector<SpacePixelFile> &drawingLayers,
               const Point2f& seed,
               const std::string& name = "All-Line Map");
    AxialPolygons m_polygons;
    prefvec<PolyConnector> m_poly_connections;
    pqvector<RadialLine> m_radial_lines;
    void setKeyVertexCount(int keyvertexcount) {
        m_keyvertexcount = keyvertexcount;
    }
    std::tuple<ShapeGraph, ShapeGraph> extractFewestLineMaps(Communicator *comm);
    void makeDivisions(const prefvec<PolyConnector>& polyconnections, const pqvector<RadialLine>& radiallines, std::map<RadialKey, pvecint> &radialdivisions, std::map<int,pvecint>& axialdividers, Communicator *comm);

};
