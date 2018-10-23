#pragma once

#include "salalib/ivga.h"
#include "salalib/pointdata.h"
#include "salalib/options.h"
#include "salalib/pixelref.h"

class VGAVisualGlobal : IVGA
{
public:
    VGAVisualGlobal();
    std::string getAnalysisName() const override {
        return "Global Visibility Analysis";
    }
    bool run(Communicator *comm, const Options &options, PointMap &map, bool simple_version) override;
};
