#include <GridGraph.hpp>
#include <ManhattanDijkstra.hpp>
#include <MultiTargetManhattanDijkstra.hpp>
#include <ProgramOptions.hpp>
#include <SimpleDijkstra.hpp>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

using pathfinding::MultiTargetManhattanDijkstra;
using pathfinding::ManhattanDijkstra;

auto main(int argc, char* argv[])
    -> int
{
    auto options = util::parseArguments(argc, argv);
    auto graph_file = options.getGraphFile();

    auto graph_opt = graph::parseFileToGridGraph(graph_file);

    auto graph = std::move(graph_opt.value());
    ManhattanDijkstra path_solver{graph};

    auto first = graph.generateRandomCellOfSize(16);
    auto second = graph.generateRandomCellOfSize(16);

    for(auto i : first) {
        for(auto j : second) {
            auto dist = path_solver.findDistance(i, j);
            if(dist != pathfinding::UNREACHABLE) {
                fmt::print("from {} to {}: {}\n", i, j, dist);
            }
        }
    }
}
