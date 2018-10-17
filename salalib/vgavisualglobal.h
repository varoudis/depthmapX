#pragma once

#include "salalib/ivga.h"
#include "salalib/pointdata.h"
#include "salalib/options.h"
#include "salalib/pixelref.h"

class VGAVisualGlobal : IVGA
{
public:
    VGAVisualGlobal();
    virtual std::string getAnalysisName() const {
        return "Global Visibility Analysis";
    }
    virtual bool run(Communicator *comm, Options& options, PointMap &map, bool simple_version);
};
