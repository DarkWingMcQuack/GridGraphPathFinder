#include <GridGraph.hpp>
#include <fmt/core.h>

auto main(int argc, char* argv[])
    -> int
{
    // fmt::print("Hello World\n");

    auto graph_opt = graph::parseFileToGridGraph("../data/BaldursGateIIScaled/AR0011SR.map");

    auto graph = std::move(graph_opt.value());

	fmt::print("{}\n", graph.isBarrier({1, 20}));
}
