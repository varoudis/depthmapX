// sala - a component of the depthmapX - spatial network analysis platform
// Copyright (C) 2000-2010, University College London, Alasdair Turner
// Copyright (C) 2011-2012, Tasos Varoudis
// Copyright (C) 2017-2018, Petros Koutsolampros

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

#pragma once

#include "salalib/ivga.h"
#include "salalib/pointdata.h"
#include "salalib/options.h"
#include "salalib/pixelref.h"

#include "genlib/simplematrix.h"

class VGAVisualGlobal : IVGA
{
public:
    std::string getAnalysisName() const override {
        return "Global Visibility Analysis";
    }
    bool run(Communicator *comm, const Options &options, PointMap &map, bool simple_version) override;
    void extractUnseen(Node& node, PixelRefVector& pixels, depthmapX::RowMatrix<int>& miscs,
                       depthmapX::RowMatrix<PixelRef>& extents) {
        for (int i = 0; i < 32; i++) {
            Bin &bin = node.bin(i);
            for (auto pixVec: bin.m_pixel_vecs) {
                for (PixelRef pix = pixVec.start(); pix.col(bin.m_dir) <= pixVec.end().col(bin.m_dir); ) {
                    int& misc = miscs(pix.y, pix.x);
                    PixelRef& extent = extents(pix.y, pix.x);
                    if (misc == 0) {
                        pixels.push_back(pix);
                        misc |= (1 << i);
                    }
                    // 10.2.02 revised --- diagonal was breaking this as it was extent in diagonal or horizontal
                    if (!(bin.m_dir & PixelRef::DIAGONAL)) {
                        if (extent.col(bin.m_dir) >= pixVec.end().col(bin.m_dir))
                            break;
                        extent.col(bin.m_dir) = pixVec.end().col(bin.m_dir);
                    }
                    pix.move(bin.m_dir);
                }
            }
        }
    }
};
