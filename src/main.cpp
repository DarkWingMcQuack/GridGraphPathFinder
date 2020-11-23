#include <filesystem>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <graph/GridGraph.hpp>
#include <pathfinding/CachingDijkstra.hpp>
#include <pathfinding/Dijkstra.hpp>
#include <selection/FullNodeSelectionCalculator.hpp>
#include <selection/SelectionLookup.hpp>
#include <separation/SeparationDistanceOracle.hpp>
#include <separation/SeparationOptimizer.hpp>
#include <separation/WellSeparationCalculator.hpp>
#include <separation/WellSeparationChecker.hpp>
#include <utils/ProgramOptions.hpp>
#include <utils/Timer.hpp>


using pathfinding::Dijkstra;
using pathfinding::CachingDijkstra;
using selection::FullNodeSelectionCalculator;
namespace fs = std::filesystem;

auto saveSeparations(const std::vector<separation::Separation>& separations,
                     const graph::GridGraph& graph,
                     std::string_view path)
{
    const auto result_path = fmt::format("{}/seps", path);
    fs::create_directories(result_path);

    for(std::size_t i{0}; i < separations.size(); i++) {
        auto file = fmt::format("{}/separation-{}", result_path, i);
        auto sep = graph.unclip(separations[i]);
        sep.toSmallFile(file);
    }
}

auto calculateSeparation(const graph::GridGraph& graph,
                         std::string_view result_folder)
    -> std::vector<separation::Separation>
{
    utils::Timer t;

    CachingDijkstra dijkstra{graph};

    auto [separations, cache] = separation::calculateSeparation(graph, dijkstra);
    const auto sepataions_before_optimization = separations.size();

    const auto distribution_file = fmt::format("{}/distribution", result_folder);
    separation::sizeDistribution3DToFile(separations, distribution_file);

    separation::SeparationOptimizer optimizer{graph,
                                              dijkstra,
                                              std::move(cache)};


    separations = optimizer.optimizeAll(std::move(separations));
    dijkstra.destroy();

    const auto message = fmt::format("runtime: {}, before optimization: |S| = {}, after |S| = {}",
                                     t.elapsed(),
                                     sepataions_before_optimization,
                                     separations.size());

    fmt::print(
        "┌{0:─^{2}}┐\n"
        "│{1: ^{2}}│\n"
        "└{0:─^{2}}┘\n",
        "",
        message,
        80);

    return separations;
}


auto loadSeparations(const graph::GridGraph& graph,
                     std::string_view folder)
    -> std::vector<separation::Separation>
{
    std::vector<separation::Separation> separations;
    for(const auto& entry : fs::directory_iterator(folder)) {

        if(entry.is_regular_file()) {
            const auto filename_str = entry.path().string();
            const auto sep_opt = separation::fromFile(filename_str);
            const auto sep = graph.toClipped(sep_opt.value());
            separations.emplace_back(sep);
        }
    }

    return separations;
}

auto runSeparation(const graph::GridGraph& graph,
                   std::vector<separation::Separation> separations,
                   std::string_view result_folder)
{
    const auto optimized_distribution_file = fmt::format("{}/optimized_distribution", result_folder);
    separation::sizeDistribution3DToFile(separations, optimized_distribution_file);


    const auto distance_file = fmt::format("{}/distance_distribution", result_folder);
    sizeToDistanceToFile(separations, graph, distance_file);

    saveSeparations(separations, graph, result_folder);

    separation::SeparationDistanceOracle oracle{graph, separations};

    //clear to save memory
    separations.clear();

    Dijkstra compare{graph};
    utils::Timer t;

    for(std::size_t i{0}; i < 50000; i++) {
        const auto from = graph.getRandomWalkableNode();
        const auto to = graph.getRandomWalkableNode();

        t.reset();
        const auto oracle_dist = oracle.findDistance(from, to);
        const auto oracle_time = t.elapsed();

        t.reset();
        const auto compare_dist = compare.findDistance(from, to);
        const auto compare_time = t.elapsed();
        fmt::print(
            "separation distance: {}\n"
            "dijkstra distance: {}\n"
            "separation time: {}\n"
            "dijkstra time: {}\n",
            oracle_dist,
            compare_dist,
            oracle_time,
            compare_time);
        fmt::print("----------------------------------------------\n");
    }
}

auto runSelection(const graph::GridGraph& graph,
                  std::string_view /*result_folder*/)
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
        const auto path = fmt::format("result-{}.seg", i);
        sel.toFile(path);
    }

    selection::SelectionLookup lookup{std::move(selections)};
}

auto main(int argc, char* argv[])
    -> int
{
    const auto options = utils::parseArguments(argc, argv);
    const auto graph_file = options.getGraphFile();
    const auto neigbour_calculator = options.getNeigbourCalculator();
    const auto graph = graph::parseFileToGridGraph(graph_file, neigbour_calculator).value();
    const auto running_mode = options.getRunningMode();
    const auto graph_filename = utils::unquote(fs::path(graph_file).filename());
    const auto result_folder = fmt::format("./results/{}/", graph_filename);


    fs::create_directories(result_folder);

    const auto message = fmt::format("File: {}\nheight: {}\nwidth: {} |V|: {}",
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

        auto separations = [&] {
            if(options.hasSeparationFolder()) {
                return loadSeparations(graph, options.getSeparationFolder());
            }
            return calculateSeparation(graph, result_folder);
        }();

        runSeparation(graph,
                      std::move(separations),
                      result_folder);
        break;
    }
    case utils::RunningMode::SELECTION: {
        runSelection(graph, result_folder);
        break;
    }
    }
}
