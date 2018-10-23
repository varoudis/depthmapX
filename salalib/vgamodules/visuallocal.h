#pragma once

#include "salalib/ivga.h"
#include "salalib/pointdata.h"
#include "salalib/options.h"
#include "salalib/pixelref.h"

class VGAVisualLocal : IVGA
{
public:
    std::string getAnalysisName() const override {
        return "Local Visibility Analysis";
    }
    bool run(Communicator *comm, const Options& options, PointMap &map, bool simple_version) override;
};
