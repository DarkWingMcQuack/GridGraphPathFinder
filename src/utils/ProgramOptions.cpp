#include <CLI/CLI.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <utils/ProgramOptions.hpp>

using utils::ProgramOptions;
using utils::RunningMode;



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

auto utils::operator>>(std::istream& in, NeigbourMetric& num)
    -> std::istream&
{
    std::string token;
    in >> token;
    if(token == "manhattan") {
        num = NeigbourMetric::MANHATTAN;
    } else {
        in.setstate(std::ios_base::failbit);
    }
    return in;
}

auto utils::operator<<(std::ostream& os, const NeigbourMetric& num)
    -> std::ostream&
{
    switch(num) {
    case NeigbourMetric::MANHATTAN:
        os << "manhattan";
        break;
    default:
        os.setstate(std::ios_base::failbit);
        break;
    }

    return os;
}

auto utils::operator>>(std::istream& in, RunningMode& num)
    -> std::istream&
{
    std::string token;
    in >> token;
    if(token == "selection") {
        num = RunningMode::SELECTION;
    } else if(token == "separation") {
        num = RunningMode::SEPARATION;
    } else {
        in.setstate(std::ios_base::failbit);
    }

    return in;
}

auto utils::operator<<(std::ostream& os, const RunningMode& num)
    -> std::ostream&
{
    switch(num) {
    case RunningMode::SELECTION:
        os << "selection";
        break;
    case RunningMode::SEPARATION:
        os << "separation";
        break;
    default:
        os.setstate(std::ios_base::failbit);
        break;
    }

    return os;
}
