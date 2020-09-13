#include <CLI/CLI.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <utils/ProgramOptions.hpp>

using utils::ProgramOptions;


ProgramOptions::ProgramOptions(std::string graph_file)
    : graph_file_(std::move(graph_file)) {}

auto ProgramOptions::getGraphFile() const
    -> std::string_view
{
    return graph_file_;
}



auto utils::parseArguments(int argc, char* argv[])
    -> ProgramOptions
{
    CLI::App app{"Grid-Graph Path Finder"};

    std::string graph_file;

    app.add_option("-g,--graph",
                   graph_file,
                   "file containing the grid graph structure")
        ->check(CLI::ExistingFile)
        ->required();

    try {
        app.parse(argc, argv);
    } catch(const CLI::ParseError& e) {
        std::exit(app.exit(e));
    }

    return ProgramOptions{std::move(graph_file)};
}
