#include <GridGraph.hpp>
#include <ProgramOptions.hpp>
#include <SimpleDijkstra.hpp>
#include <fmt/core.h>
#include <fmt/ostream.h>

using pathfinding::SimpleDijkstra;

auto main(int argc, char* argv[])
    -> int
{
    auto options = util::parseArguments(argc, argv);
    auto graph_file = options.getGraphFile();

    auto graph_opt = graph::parseFileToGridGraph(graph_file);

    auto graph = std::move(graph_opt.value());
    SimpleDijkstra path_solver{graph};

    auto first = graph.generateRandomCellOfSize(16);
    auto second = graph.generateRandomCellOfSize(16);

    fmt::print("first:\n{}\n", first);
    fmt::print("second:\n{}\n", second);

    for(auto i : first) {
        for(auto j : second) {
            auto distance = path_solver.findDistance(i, j);
            fmt::print("distance between {} and {}: {}\n", i, j, distance);
        }
    }
}
