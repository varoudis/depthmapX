#ifndef __GMLMAP_H__
#define __GMLMAP_H__

typedef prefvec<pvecpoint> polyset;

class GMLMap
{
protected:
   QtRegion m_region;
public:
   pqmap<pstring,polyset> m_keys;
public:
   GMLMap() {;}
   bool parse(const pvecstring& fileset, Communicator *communicator);
   Point2f getBottomLeft()
   { return m_region.bottom_left; }
   Point2f getTopRight()
   { return m_region.top_right; }
   QtRegion getRegion()
   { return m_region; }
};

#endif