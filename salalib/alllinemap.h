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
    AllLineMap(const std::string& name = "All-Line Map"):
        ShapeGraph(name, ShapeMap::ALLLINEMAP) {}
    AxialPolygons m_polygons;
    prefvec<PolyConnector> m_poly_connections;
    std::vector<RadialLine> m_radial_lines;
    void setKeyVertexCount(int keyvertexcount) {
        m_keyvertexcount = keyvertexcount;
    }
    std::tuple<std::unique_ptr<ShapeGraph>, std::unique_ptr<ShapeGraph>> extractFewestLineMaps(Communicator *comm);
    void makeDivisions(const prefvec<PolyConnector>& polyconnections, const std::vector<RadialLine> &radiallines,
                       std::map<RadialKey, std::set<int> > &radialdivisions, std::map<int, std::set<int> > &axialdividers,
                       Communicator *comm);

};
