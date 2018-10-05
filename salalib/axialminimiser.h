#pragma once

#include "salalib/alllinemap.h"

struct ValueTriplet
{
   int value1;
   float value2;
   int index;
};

class AxialMinimiser
{
protected:
   AllLineMap *m_alllinemap;
   //
   ValueTriplet *m_vps;
   bool *m_removed;
   bool *m_affected;
   bool *m_vital;
   int *m_radialsegcounts;
   int *m_keyvertexcounts;
   std::vector<Connector> m_axialconns; // <- uses a copy of axial lines as it will remove connections
public:
   AxialMinimiser(const AllLineMap& alllinemap, int no_of_axsegcuts, int no_of_radialsegs);
   ~AxialMinimiser();
   void removeSubsets(std::map<int, std::set<int> > &axsegcuts, std::map<RadialKey,RadialSegment>& radialsegs,
                      std::map<RadialKey, std::set<int> >& rlds, std::vector<RadialLine> &radial_lines,
                      std::vector<std::vector<int> > &keyvertexconns, int *keyvertexcounts);
   void fewestLongest(std::map<int,std::set<int> >& axsegcuts, std::map<RadialKey,RadialSegment>& radialsegs,
                      std::map<RadialKey, std::set<int> > &rlds, std::vector<RadialLine>& radial_lines,
                      std::vector<std::vector<int> >& keyvertexconns, int *keyvertexcounts);
   // advanced topological testing:
   bool checkVital(int checkindex, std::set<int> &axSegCut, std::map<RadialKey,RadialSegment>& radialsegs,
                   std::map<RadialKey, std::set<int> > &rlds, std::vector<RadialLine> &radial_lines);
   //
   bool removed(int i) const
   { return m_removed[i]; }
};
