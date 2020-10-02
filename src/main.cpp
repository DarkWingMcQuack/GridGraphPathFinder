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
    ManhattanDijkstra path_finder{graph_opt.value()};

    // FullNodeSelectionCalculator<ManhattanDijkstra> selection_calculator{graph};

    fmt::print("height: {}\n", graph.height);
    fmt::print("width: {}\n", graph.width);
    utils::Timer t;
    // auto selections = selection_calculator.calculateFullNodeSelection();
    auto separations = separation::calculateSeparation(path_finder);

    fmt::print("runtime: {}\n", t.elapsed());

    fmt::print("selections calculated: {}\n", separations.size());

    std::sort(std::rbegin(separations),
              std::rend(separations));

    auto isSane = separation::test::separationSanityCheck(graph, separations);

    fmt::print("IS SANE: {}\n", isSane);

    int counter{0};
    for(const auto& seg : separations) {
        auto path = fmt::format("result-{}.seg", counter++);
        // seg.toFile(path);
        separation::toFile(seg, path);
    }

    // selection.value().toFile("result.seg");
}
