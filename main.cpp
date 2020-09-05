#include <GridGraph.hpp>
#include <ProgramOptions.hpp>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <SimpleDijkstra.hpp>
#include <WellSeperationChecker.hpp>

using pathfinding::SimpleDijkstra;
using preprocessing::WellSeperationChecker;

auto main(int argc, char* argv[])
    -> int
{
    auto options = util::parseArguments(argc, argv);
    auto graph_file = options.getGraphFile();

    auto graph_opt = graph::parseFileToGridGraph(graph_file);

    auto graph = std::move(graph_opt.value());
	SimpleDijkstra path_solver{graph};
	WellSeperationChecker checker{std::move(path_solver), graph};

	auto first = graph.generateRandomCellOfSize(4);
	auto second = graph.generateRandomCellOfSize(4);

	fmt::print("first:\n{}\n", first);
	fmt::print("second:\n{}\n", second);

    fmt::print("is well separated {} \n",
			   (bool)checker.checkIfWellSeparated(first, second));
}
