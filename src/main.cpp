#include <filesystem>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <graph/GridGraph.hpp>
#include <pathfinding/CachingDijkstra.hpp>
#include <pathfinding/Dijkstra.hpp>
#include <selection/FullNodeSelectionCalculator.hpp>
#include <selection/HubLabelSelectionLookup.hpp>
#include <separation/WellSeparationCalculator.hpp>
#include <separation/WellSeparationChecker.hpp>
#include <utils/ProgramOptions.hpp>
#include <utils/Timer.hpp>


using pathfinding::Dijkstra;
using pathfinding::CachingDijkstra;
using selection::FullNodeSelectionCalculator;
namespace fs = std::filesystem;

auto runSeparation(const graph::GridGraph& graph,
                   std::string_view result_folder)
{
    utils::Timer t;

    auto separations = separation::calculateSeparation<CachingDijkstra>(graph);
    auto message = fmt::format("runtime: {}, |S| = {}", t.elapsed(), separations.size());

    fmt::print(
        "┌{0:─^{2}}┐\n"
        "│{1: ^{2}}│\n"
        "└{0:─^{2}}┘\n",
        "",
        message,
        80);

    std::sort(std::rbegin(separations),
              std::rend(separations));

    auto distribution_file = fmt::format("{}/distribution", result_folder);
    separation::sizeDistribution3DToFile(separations, distribution_file);

    // for(std::size_t i{0}; i < separations.size(); i++) {
    //     const auto& sep = separations[i];
    //     auto path = fmt::format("{}/result-{}.seg", result_folder, i);
    //     separation::toFile(graph.unclip(sep), path);
    // }
}

auto runSelection(const graph::GridGraph& graph,
                  std::string_view result_folder)
{
    utils::Timer t;
    FullNodeSelectionCalculator<Dijkstra> selection_calculator{graph};
    auto selections = selection_calculator.calculateFullNodeSelection();

    fmt::print("runtime: {}\n", t.elapsed());
    fmt::print("selections calculated: {}\n", selections.size());

    std::sort(std::rbegin(selections),
              std::rend(selections));


    for(std::size_t i{0}; i < selections.size(); i++) {
        const auto& sel = selections[i];
        auto path = fmt::format("result-{}.seg", i);
        sel.toFile(path);
    }

    selection::HubLabelSelectionLookup lookup{std::move(selections)};
}


auto main(int argc, char* argv[])
    -> int
{
    auto options = utils::parseArguments(argc, argv);
    auto graph_file = options.getGraphFile();
    auto neigbour_calculator = options.getNeigbourCalculator();
    auto graph = graph::parseFileToGridGraph(graph_file, neigbour_calculator).value();
    auto running_mode = options.getRunningMode();
    auto graph_filename = utils::unquote(fs::path(graph_file).filename());
    auto result_folder = fmt::format("./results/{}/", graph_filename);

    fs::create_directories(result_folder);

    auto message = fmt::format("File: {}\nheight: {}\nwidth: {} |V|: {}",
                               graph_file,
                               graph.getHeight(),
                               graph.getWidth(),
                               graph.countWalkableNodes());

    fmt::print(
        "┌{0:─^{6}}┐\n"
        "  File: {1: <{6}}\n"
        "  Height: {2: <{6}}\n"
        "  Width: {3: <{6}}\n"
        "  #Nodes: {4: <{6}}\n"
        "  #active Nodes: {5: <{6}}\n"
        "└{0:─^{6}}┘\n",
        "",
        graph_file,
        graph.getHeight(),
        graph.getWidth(),
        graph.getHeight() * graph.getWidth(),
        graph.countWalkableNodes(),
        80);

    switch(running_mode) {

    case utils::RunningMode::SEPARATION: {
        runSeparation(graph, result_folder);
        break;
    }
    case utils::RunningMode::SELECTION: {
        runSelection(graph, result_folder);
        break;
    }
    }
}
