#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <graph/GridGraph.hpp>
#include <pathfinding/Dijkstra.hpp>
#include <selection/FullNodeSelectionCalculator.hpp>
#include <selection/HubLabelSelectionLookup.hpp>
#include <separation/WellSeparationCalculator.hpp>
#include <separation/WellSeparationChecker.hpp>
#include <utils/ProgramOptions.hpp>
#include <utils/Timer.hpp>


using pathfinding::Dijkstra;
using selection::FullNodeSelectionCalculator;

auto runSeparation(const graph::GridGraph& graph)
{
    utils::Timer t;
    auto separations = separation::calculateSeparation<Dijkstra>(graph);
    fmt::print("runtime: {}\n", t.elapsed());
    fmt::print("separations calculated: {}\n", separations.size());

    std::sort(std::rbegin(separations),
              std::rend(separations));

    for(std::size_t i{0}; i < separations.size(); i++) {
        const auto& sep = separations[i];
        auto path = fmt::format("result-{}.seg", i);
        separation::toFile(graph.unclip(sep), path);
    }

    fmt::print("sound?: {}\n", separation::test::separationSanityCheck(graph, separations));

    for(std::size_t i{0}; i < separations.size(); i++) {
        auto sep = separations[i];
        auto left = separation::getFirstCluster(sep);
        auto right = separation::getSecondCluster(sep);

        for(std::size_t j{0}; j < separations.size(); j++) {
            auto inner_sep = separations[j];
            // if(sep == inner_sep) {
            //     continue;
            // }
            if(i == j) {
                continue;
            }

            auto inner_left = separation::getFirstCluster(inner_sep);
            auto inner_right = separation::getSecondCluster(inner_sep);

            if((left.isSubSetOf(inner_left) and right.isSubSetOf(inner_right))
               or (left.isSubSetOf(inner_right) and right.isSubSetOf(inner_left))
               or (left == inner_left and right == inner_right)
               or (left == inner_right and right == inner_left)) {
                fmt::print("FOUND A FUCKIN FLAW {} and {}\n", i, j);
            }
        }
    }
}

auto runSelection(const graph::GridGraph& graph)
{
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

    selection::HubLabelSelectionLookup lookup{std::move(selections)};
}

auto main(int argc, char* argv[])
    -> int
{
    auto options = utils::parseArguments(argc, argv);
    auto graph_file = options.getGraphFile();
    auto neigbour_calculator = options.getNeigbourCalculator();
    auto graph = graph::parseFileToGridGraph(graph_file, neigbour_calculator).value();
    auto running_mode = options.getRunningMode();

    switch(running_mode) {

    case utils::RunningMode::SEPARATION: {
        runSeparation(graph);
        break;
    }
    case utils::RunningMode::SELECTION: {
        runSelection(graph);
        break;
    }
    }
}
