#include <GridGraph.hpp>
#include <ManhattanDijkstra.hpp>
#include <MultiTargetManhattanDijkstra.hpp>
#include <ProgramOptions.hpp>
#include <SimpleDijkstra.hpp>
#include <WellSeparationChecker.hpp>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

using pathfinding::MultiTargetManhattanDijkstra;
using pathfinding::ManhattanDijkstra;
using separation::WellSeperationChecker;

auto main(int argc, char* argv[])
    -> int
{
    auto options = util::parseArguments(argc, argv);
    auto graph_file = options.getGraphFile();

    auto graph_opt = graph::parseFileToGridGraph(graph_file);

    auto graph = std::move(graph_opt.value());
    ManhattanDijkstra single_target_path_solver{graph};
    MultiTargetManhattanDijkstra multi_path_solver{graph};

    auto first = graph.generateRandomCellOfSize(16);
    auto second = graph.generateRandomCellOfSize(16);

    WellSeperationChecker separation_checker{std::move(multi_path_solver),
                                             std::move(single_target_path_solver),
                                             graph,
                                             first,
                                             second};

    auto are_separated = separation_checker.checkIfWellSeparated(first, second);

    fmt::print("are the two well separate? {}\n", are_separated);
}
