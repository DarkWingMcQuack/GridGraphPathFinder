#include <CLI/CLI.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <utils/ProgramOptions.hpp>

using utils::ProgramOptions;
using utils::RunningMode;
using std::string_literals::operator""s;



ProgramOptions::ProgramOptions(std::string graph_file,
                               NeigbourMetric neigbour_mode,
                               RunningMode running_mode)
    : graph_file_(std::move(graph_file)),
      neigbour_mode_(neigbour_mode),
      running_mode_(running_mode) {}

auto ProgramOptions::getGraphFile() const noexcept
    -> std::string_view
{
    return graph_file_;
}

auto ProgramOptions::getRunningMode() const noexcept
    -> RunningMode
{
    return running_mode_;
}

auto ProgramOptions::getNeigbourCalculator() const noexcept
    -> graph::NeigbourCalculator
{
    switch(neigbour_mode_) {
    case utils::NeigbourMetric::ALL_SURROUNDING:
        return graph::AllSouroundingNeigbourCalculator{};
    default:
        return graph::ManhattanNeigbourCalculator{};
    }
}


auto utils::parseArguments(int argc, char* argv[])
    -> ProgramOptions
{
    CLI::App app{"Grid-Graph Path Finder"};
    static const std::unordered_map mode_map{std::pair{"separation"s, RunningMode::SEPARATION},
                                             std::pair{"selection"s, RunningMode::SELECTION}};

    static const std::unordered_map neigbour_map{std::pair{"manhattan"s, NeigbourMetric::MANHATTAN},
                                                 std::pair{"all-sourounding"s, NeigbourMetric::ALL_SURROUNDING}};

    std::string graph_file;
    auto mode = RunningMode::SEPARATION;
    auto neigbours = NeigbourMetric::MANHATTAN;

    app.add_option("-g,--graph",
                   graph_file,
                   "file containing the grid graph structure")
        ->check(CLI::ExistingFile)
        ->required();

    app.add_option("-m,--mode",
                   mode,
                   "preprocessing mode")
        ->transform(CLI::CheckedTransformer(mode_map, CLI::ignore_case))
        ->required();

    app.add_option("-n,--neigbour-mode",
                   neigbours,
                   "neigbour mode")
        ->transform(CLI::CheckedTransformer(neigbour_map, CLI::ignore_case));

    try {
        app.parse(argc, argv);
    } catch(const CLI::ParseError& e) {
        std::exit(app.exit(e));
    }

    return ProgramOptions{std::move(graph_file),
                          neigbours,
                          mode};
}
