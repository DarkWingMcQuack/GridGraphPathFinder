#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <graph/GridGraph.hpp>
#include <pathfinding/ManhattanDijkstra.hpp>
#include <pathfinding/NormalDijkstra.hpp>
#include <selection/FullNodeSelectionCalculator.hpp>
#include <separation/WellSeparationCalculator.hpp>
#include <separation/WellSeparationChecker.hpp>
#include <utils/ProgramOptions.hpp>
#include <utils/Timer.hpp>


using pathfinding::ManhattanDijkstra;
using selection::FullNodeSelectionCalculator;


auto main(int argc, char* argv[])
    -> int
{
    auto options = utils::parseArguments(argc, argv);
    auto graph_file = options.getGraphFile();
    auto graph_opt = graph::parseFileToGridGraph(graph_file);
    const auto& graph = graph_opt.value();
    auto running_mode = options.getRunningMode();

    ManhattanDijkstra path_finder{graph_opt.value()};

    // FullNodeSelectionCalculator<ManhattanDijkstra> selection_calculator{graph};
    switch(running_mode) {
    case utils::RunningMode::SEPARATION: {

        utils::Timer t;
        auto separations = separation::calculateSeparation(path_finder);
        fmt::print("runtime: {}\n", t.elapsed());
        fmt::print("separations calculated: {}\n", separations.size());

        std::sort(std::rbegin(separations),
                  std::rend(separations));

        for(std::size_t i{0}; i < separations.size(); i++) {
            const auto& sep = separations[i];
            auto path = fmt::format("result-{}.seg", i);
            separation::toFile(sep, path);
        }

        break;
    }
    case utils::RunningMode::SELECTION: {
        FullNodeSelectionCalculator<ManhattanDijkstra> selection_calculator{graph};
        auto selections = selection_calculator.calculateFullNodeSelection();

        std::sort(std::rbegin(selections),
                  std::rend(selections));

        for(std::size_t i{0}; i < selections.size(); i++) {
            const auto& sel = selections[i];
            auto path = fmt::format("result-{}.seg", i);
            sel.toFile(path);
        }
    }
    }
}
