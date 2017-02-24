#include "runmethods.h"
#include "salalib/mgraph.h"
#include "radiusconverter.h"
#include "exceptions.h"
#include "simpletimer.h"
#include <memory>
#include <sstream>

namespace dm_runmethods
{

    void runVga(const CommandLineParser &cmdP, const IRadiusConverter &converter)
    {
        MetaGraph mgraph;
        auto result = mgraph.read(cmdP.getFileName().c_str());
        if ( result != MetaGraph::OK)
        {
            std::stringstream message;
            message << "Failed to load graph from file " << cmdP.getFileName() << ", error " << result << flush;
            throw depthmapX::RuntimeException(message.str().c_str());
        }
        std::unique_ptr<Communicator> comm(new ICommunicator());
        std::unique_ptr<Options> options(new Options());

        switch(cmdP.getVgaMode())
        {
            case VgaMode::VISBILITY:
                options->output_type = Options::OUTPUT_VISUAL;
                options->local = cmdP.localMeasures();
                options->global = cmdP.globalMeasures();
                options->radius = converter.ConvertForVisibility(cmdP.getRadius());
                break;
            case VgaMode::METRIC:
                options->output_type = Options::OUTPUT_METRIC;
                options->radius = converter.ConvertForMetric(cmdP.getRadius());
                break;
            case VgaMode::ANGULAR:
                options->output_type = Options::OUTPUT_ANGULAR;
                break;
            case VgaMode::ISOVIST:
                options->output_type = Options::OUTPUT_ISOVIST;
            default:
                throw depthmapX::SetupCheckException("Unsupported VGA mode");
        }
        SimpleTimer timer;
        mgraph.analyseGraph(comm.get(), *options, cmdP.simpleMode() );
        std::cout << "Analysis took " << timer.getTimeInSeconds() << " seconds." << std::endl;
        mgraph.write(cmdP.getOuputFile().c_str(),METAGRAPH_VERSION, false);
    }

}
