#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <graph/GridGraph.hpp>
#include <pathfinding/ManhattanDijkstra.hpp>
#include <pathfinding/NormalDijkstra.hpp>
#include <separation/NodeSelectionCalculator.hpp>
#include <separation/WellSeparationCalculator.hpp>
#include <separation/WellSeparationChecker.hpp>
#include <utils/ProgramOptions.hpp>
#include <utils/Timer.hpp>

using pathfinding::ManhattanDijkstra;

auto main(int argc, char* argv[])
    -> int
{
    auto options = utils::parseArguments(argc, argv);
    auto graph_file = options.getGraphFile();

    auto graph_opt = graph::parseFileToGridGraph(graph_file);
    const auto& graph = graph_opt.value();
    ManhattanDijkstra path_finder{graph_opt.value()};

    // auto left = graph.getRandomWalkableNode();
    // auto right = graph.getRandomWalkableNode();
    separation::NodeSelectionCalculator<ManhattanDijkstra> selection_calculator{graph};

    utils::Timer t;
    // auto selection = selection_calculator.calculateFullSelection(left, right);

    auto separations = separation::calculateSeparation(std::move(path_finder));

    fmt::print("runtime: {}\n", t.elapsed());
    // fmt::print("selection size: {}\n",
    //            selection.value().getLeftSelection().size()
    //                + selection.value().getRightSelection().size());
    // fmt::print("number of nodes: {}\n",
    //            graph_opt.value().width * graph_opt.value().height);
    auto max_segment = *std::max_element(std::begin(separations),
                                         std::end(separations),
                                         [&](const auto& lhs, const auto& rhs) {
                                             return graph.countNumberOfWalkableNodes(lhs.getFirstCluster())
                                                 * graph.countNumberOfWalkableNodes(lhs.getSecondCluster())
                                                 < graph.countNumberOfWalkableNodes(rhs.getFirstCluster())
                                                 * graph.countNumberOfWalkableNodes(rhs.getSecondCluster());
                                         });

    fmt::print("separations calculated: {}\n", separations.size());

    max_segment.toFile("result.seg");
}
