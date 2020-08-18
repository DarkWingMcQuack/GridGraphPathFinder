#include <GridGraph.hpp>
#include <ProgramOptions.hpp>
#include <fmt/core.h>

auto main(int argc, char* argv[])
    -> int
{
    auto options = util::parseArguments(argc, argv);
    auto graph_file = options.getGraphFile();

    auto graph_opt = graph::parseFileToGridGraph(graph_file);

    auto graph = std::move(graph_opt.value());
    fmt::print("width: {} height: {}\n",
               graph.width,
               graph.height);
}
