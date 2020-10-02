#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <graph/GridGraph.hpp>
#include <pathfinding/Dijkstra.hpp>
#include <selection/FullNodeSelectionCalculator.hpp>
#include <separation/WellSeparationCalculator.hpp>
#include <separation/WellSeparationChecker.hpp>
#include <utils/ProgramOptions.hpp>
#include <utils/Timer.hpp>


using pathfinding::Dijkstra;
using selection::FullNodeSelectionCalculator;

auto main(int argc, char* argv[])
    -> int
{
    auto options = utils::parseArguments(argc, argv);
    auto graph_file = options.getGraphFile();
    auto neigbour_mode = options.getNeigbourMode();

    auto neigbour_calculator = [&]() -> graph::NeigbourCalculator {
        switch(neigbour_mode) {
        case utils::NeigbourMetric::MANHATTAN:
            return graph::NeigbourCalculator{};
        }
    }();

    auto graph_opt = graph::parseFileToGridGraph(graph_file, neigbour_calculator);
    const auto& graph = graph_opt.value();
    auto running_mode = options.getRunningMode();

    Dijkstra path_finder{graph_opt.value()};

    // FullNodeSelectionCalculator<Dijkstra> selection_calculator{graph};
    switch(running_mode) {
    case utils::RunningMode::SEPARATION: {

        utils::Timer t;
        auto separations = separation::calculateSeparation<Dijkstra>(graph);
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

        utils::Timer t;
        FullNodeSelectionCalculator<Dijkstra> selection_calculator{graph};
        auto selections = selection_calculator.calculateFullNodeSelection();

        fmt::print("runtime: {}\n", t.elapsed());
        fmt::print("selections calculated: {}\n", selections.size());

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
