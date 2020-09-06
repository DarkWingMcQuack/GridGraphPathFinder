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

auto main(int argc, char* argv[])
    -> int
{
    auto options = util::parseArguments(argc, argv);
    auto graph_file = options.getGraphFile();

    auto graph_opt = graph::parseFileToGridGraph(graph_file);

    auto graph = std::move(graph_opt.value());
    ManhattanDijkstra single_target_path_solver{graph};
    MultiTargetManhattanDijkstra multi_path_solver{graph};

    auto first = graph.generateRandomCellOfSize(100);
    auto second = graph.generateRandomCellOfSize(100);


    bool separated{false};
    auto compares{0};
    while(!separated) {
        first = graph.generateRandomCellOfSize(20);
        second = graph.generateRandomCellOfSize(20);
        if(!graph.hasWalkableNode(first)) {
            continue;
        }

        if(!graph.hasWalkableNode(second)) {
            continue;
        }
        compares++;
        fmt::print("---------------------------------------------------------------------\n");
        auto separation = separation::checkSeparation(multi_path_solver,
                                                      first,
                                                      second);
        separated = (bool)separation;
        fmt::print("are the two well separate? {}\n", separated);
        fmt::print("---------------------------------------------------------------------\n");
        if(separated) {
            fmt::print("compared {} random clusters\n", compares);
            fmt::print("first center: {}\nsecond center: {}\ndistance: {}\n",
                       separation.value().getFirstClusterCenter(),
                       separation.value().getSecondClusterCenter(),
                       separation.value().getCenterDistance());
        }
    }
}
