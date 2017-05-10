// Copyright (C) 2017 Christian Sailer

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

#include "runmethods.h"
#include "salalib/mgraph.h"
#include "salalib/linkutils.h"
#include "radiusconverter.h"
#include "exceptions.h"
#include "simpletimer.h"
#include <memory>
#include <sstream>
#include <vector>

namespace dm_runmethods
{

    std::unique_ptr<MetaGraph> loadGraph(const pstring& filename) {
        std::unique_ptr<MetaGraph> mgraph(new MetaGraph);
        std::cout << "Loading graph " << filename << std::flush; 
        auto result = mgraph->read(filename);
        if ( result != MetaGraph::OK)
        {
            std::stringstream message;
            message << "Failed to load graph from file " << filename << ", error " << result << flush;
            throw depthmapX::RuntimeException(message.str().c_str());
        }
        std::cout << " ok\n" << std::flush;
        return mgraph;
    }

    void linkGraph(const CommandLineParser &cmdP)
    {
        auto mgraph = loadGraph(cmdP.getFileName().c_str());
        PointMap& currentMap = mgraph->getDisplayedPointMap();

        vector<PixelRefPair> newLinks = depthmapX::getLinksFromMergeLines(cmdP.linkOptions().getMergeLines(), currentMap);

        for (size_t i = 0; i < newLinks.size(); i++)
        {
            PixelRefPair link = newLinks.at(i);
            currentMap.mergePixels(link.a,link.b);
        }
        mgraph->write(cmdP.getOuputFile().c_str(),METAGRAPH_VERSION, false);
    }

    void runVga(const CommandLineParser &cmdP, const IRadiusConverter &converter)
    {
        auto mgraph = loadGraph(cmdP.getFileName().c_str());

        std::unique_ptr<Communicator> comm(new ICommunicator());
        std::unique_ptr<Options> options(new Options());

        cout << "Getting options..." << std::flush;
        switch(cmdP.vgaOptions().getVgaMode())
        {
            case VgaParser::VgaMode::VISBILITY:
                options->output_type = Options::OUTPUT_VISUAL;
                options->local = cmdP.vgaOptions().localMeasures();
                options->global = cmdP.vgaOptions().globalMeasures();
                if (options->global )
                {
                    options->radius = converter.ConvertForVisibility(cmdP.vgaOptions().getRadius());
                }
                break;
            case VgaParser::VgaMode::METRIC:
                options->output_type = Options::OUTPUT_METRIC;
                options->radius = converter.ConvertForMetric(cmdP.vgaOptions().getRadius());
                break;
            case VgaParser::VgaMode::ANGULAR:
                options->output_type = Options::OUTPUT_ANGULAR;
                break;
            case VgaParser::VgaMode::ISOVIST:
                options->output_type = Options::OUTPUT_ISOVIST;
            default:
                throw depthmapX::SetupCheckException("Unsupported VGA mode");
        }
        cout << " ok\nAnalysing graph..." << std::flush;
        SimpleTimer timer;
        mgraph->analyseGraph(comm.get(), *options, cmdP.simpleMode() );
        std::cout << " ok\nAnalysis took " << timer.getTimeInSeconds() << " seconds." << std::endl;
        std::cout << "Writing out result..." << std::flush;
        mgraph->write(cmdP.getOuputFile().c_str(),METAGRAPH_VERSION, false);
        std::cout << " ok" << std::endl;
    }

}
