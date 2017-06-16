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
#include <salalib/gridproperties.h>

namespace dm_runmethods
{


#define CONCAT_(x,y) x##y
#define CONCAT(x,y) CONCAT_(x,y)
#define DO_TIMED(message, code)\
    SimpleTimer CONCAT(t_, __LINE__); \
    code; \
    perfWriter.addData(message, CONCAT(t_, __LINE__).getTimeInSeconds());


    std::unique_ptr<MetaGraph> loadGraph(const pstring& filename, IPerformanceSink &perfWriter) {
        std::unique_ptr<MetaGraph> mgraph(new MetaGraph);
        std::cout << "Loading graph " << filename << std::flush;
        DO_TIMED( "Load graph file", auto result = mgraph->read(filename);)
        if ( result != MetaGraph::OK)
        {
            std::stringstream message;
            message << "Failed to load graph from file " << filename << ", error " << result << flush;
            throw depthmapX::RuntimeException(message.str().c_str());
        }
        std::cout << " ok\n" << std::flush;
        return mgraph;
    }

    void linkGraph(const CommandLineParser &cmdP, const std::vector<Line> &mergeLines, IPerformanceSink &perfWriter)
    {
        auto mgraph = loadGraph(cmdP.getFileName().c_str(), perfWriter);
        SimpleTimer t;
        PointMap& currentMap = mgraph->getDisplayedPointMap();

        vector<PixelRefPair> newLinks = depthmapX::pixelateMergeLines(mergeLines, currentMap);
        depthmapX::mergePixelPairs(newLinks, currentMap);

        perfWriter.addData("Linking graph", t.getTimeInSeconds());
        DO_TIMED("Writing graph", mgraph->write(cmdP.getOuputFile().c_str(),METAGRAPH_VERSION, false);)
    }

    void runVga(const CommandLineParser &cmdP, const VgaParser &vgaP, const IRadiusConverter &converter, IPerformanceSink &perfWriter)
    {
        auto mgraph = loadGraph(cmdP.getFileName().c_str(), perfWriter);

        std::unique_ptr<Communicator> comm(new ICommunicator());
        std::unique_ptr<Options> options(new Options());

        cout << "Getting options..." << std::flush;
        switch(vgaP.getVgaMode())
        {
            case VgaParser::VgaMode::VISBILITY:
                options->output_type = Options::OUTPUT_VISUAL;
                options->local = vgaP.localMeasures();
                options->global = vgaP.globalMeasures();
                if (options->global )
                {
                    options->radius = converter.ConvertForVisibility(vgaP.getRadius());
                }
                break;
            case VgaParser::VgaMode::METRIC:
                options->output_type = Options::OUTPUT_METRIC;
                options->radius = converter.ConvertForMetric(vgaP.getRadius());
                break;
            case VgaParser::VgaMode::ANGULAR:
                options->output_type = Options::OUTPUT_ANGULAR;
                break;
            case VgaParser::VgaMode::ISOVIST:
                options->output_type = Options::OUTPUT_ISOVIST;
                break;
            case VgaParser::VgaMode::THRU_VISION:
                options->output_type = Options::OUTPUT_THRU_VISION;
                break;
            default:
                throw depthmapX::SetupCheckException("Unsupported VGA mode");
        }
        cout << " ok\nAnalysing graph..." << std::flush;

        DO_TIMED("Run VGA", mgraph->analyseGraph(comm.get(), *options, cmdP.simpleMode() ))
        std::cout << " ok\nWriting out result..." << std::flush;
        DO_TIMED("Writing graph", mgraph->write(cmdP.getOuputFile().c_str(),METAGRAPH_VERSION, false))
        std::cout << " ok" << std::endl;
    }

    void fillGraph(MetaGraph& graph, const Point2f& point)
    {
        auto r = graph.getRegion();
        if (!r.contains(point))
        {
            throw depthmapX::RuntimeException("Point outside of target region");
        }
        graph.makePoints(point, 0, 0);
    }

    void runVisualPrep(
            const CommandLineParser &clp,
            double gridSize, const std::vector<Point2f> &fillPoints,
            double maxVisibility,
            bool boundaryGraph,
            IPerformanceSink &perfWriter)
    {
        auto mGraph = loadGraph(clp.getFileName().c_str(),perfWriter);

        std::cout << "Initial checks... " << std::flush;
        auto state = mGraph->getState();
        if (~state & MetaGraph::LINEDATA)
        {
            throw depthmapX::RuntimeException("Graph must have line data before preparing VGA");
        }
        // set grid
        QtRegion r = mGraph->getRegion();

        GridProperties gp(__max(r.width(), r.height()));
        if ( gridSize > gp.getMax() ||  gridSize < gp.getMin())
        {
            std::stringstream message;
            message << "Chosen grid spacing " << gridSize << " is outside of the expected interval of "
                    << gp.getMin() << " <= spacing <= " << gp.getMax() << std::flush;
            throw depthmapX::RuntimeException(message.str());
        }

        std::cout << "ok\nSetting up grid... " << std::flush;
        if (!mGraph->PointMaps::size() || mGraph->getDisplayedPointMap().isProcessed()) {
           // this can happen if there are no displayed maps -- so flag new map required:
            mGraph->addNewMap();
        }
        DO_TIMED("Setting grid", mGraph->setGrid(gridSize, Point2f(0.0, 0.0)))

        std::cout << "ok\nFilling grid... " << std::flush;
        DO_TIMED("Filling grid",
                 for_each(fillPoints.begin(), fillPoints.end(), [&mGraph](const Point2f &point)->void{fillGraph(*mGraph, point);}))


        std::cout << "ok\nCalculating connectivity... " << std::flush;
        DO_TIMED("Calculate Connectivity", mGraph->makeGraph(0, boundaryGraph ? 1 : 0, maxVisibility))
        std::cout << " ok\nWriting out result..." << std::flush;
        DO_TIMED("Writing graph", mGraph->write(clp.getOuputFile().c_str(),METAGRAPH_VERSION, false))
                std::cout << " ok" << std::endl;
    }

    void runAxialAnalysis(const CommandLineParser &clp, const AxialParser &ap, IPerformanceSink &perfWriter)
    {
        auto mGraph = loadGraph(clp.getFileName().c_str(), perfWriter);

        auto state = mGraph->getState();
        if ( ap.runAllLines())
        {
           if (~state & MetaGraph::LINEDATA)
           {
               throw depthmapX::RuntimeException("Line drawing must be loaded before axial map can be constructed");
           }
           std::cout << "Making all line map... " << std::flush;
           DO_TIMED("Making all axes map", for_each (ap.getAllAxesRoots().begin(),ap.getAllAxesRoots().end(), [&mGraph](const Point2f &point)->void{mGraph->makeAllLineMap(0, point);} ))
           std::cout << "ok" << std::endl;
        }

        if (ap.runFewestLines())
        {
            if (~state & MetaGraph::LINEDATA)
            {
                throw depthmapX::RuntimeException("Line drawing must be loaded before fewest line map can be constructed");
            }
            if (!mGraph->hasAllLineMap())
            {
                throw depthmapX::RuntimeException("All line map must be constructed before fewest lines can be constructed. Use -aa to do this");
            }
            std::cout << "Constructing fewest line map... " << std::flush;
            DO_TIMED("Fewest line map", mGraph->makeFewestLineMap(0,1))
            std::cout << "ok" << std::endl;
        }
        std::cout << "Writing out result..." << std::flush;
        DO_TIMED("Writing graph", mGraph->write(clp.getOuputFile().c_str(),METAGRAPH_VERSION, false))
        std::cout << " ok" << std::endl;

    }




}
