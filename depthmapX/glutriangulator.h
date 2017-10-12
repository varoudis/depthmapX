#pragma once

#include "genlib/p2dpoly.h"
#include <vector>

#ifdef __linux__
#include "GL/glu.h"
#else
#include "glu.h"
#endif

class GLUTriangulator
{
public:
    static vector< Point2f > triangulate(const vector< Point2f >& polygon);
};
