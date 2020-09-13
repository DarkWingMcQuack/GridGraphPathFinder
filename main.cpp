#include <Dijkstra.hpp>
#include <GridGraph.hpp>
#include <PathQuerySystem.hpp>
#include <ProgramOptions.hpp>
#include <WellSeparationCalculator.hpp>
#include <WellSeparationChecker.hpp>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

using pathfinding::Dijkstra;

auto main(int argc, char* argv[])
    -> int
{
    auto options = util::parseArguments(argc, argv);
    auto graph_file = options.getGraphFile();

    auto graph_opt = graph::parseFileToGridGraph(graph_file);
    Dijkstra path_finder{graph_opt.value()};

    auto separations = separation::calculateSeparation(std::move(path_finder));

    fmt::print("number of separations: {}\n", separations.size());
}
