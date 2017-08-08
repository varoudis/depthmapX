#pragma once

#include "genlib/p2dpoly.h"
#include "gl.h"
#include "glu.h"
#include <vector>

class GLUTriangulator
{
public:
    static vector< Point2f > triangulate(const vector< Point2f >& polygon);
};
