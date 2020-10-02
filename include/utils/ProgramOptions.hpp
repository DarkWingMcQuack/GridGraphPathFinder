#pragma once

#include <optional>
#include <string>
#include <iostream>
#include <string_view>

namespace utils {

enum class NeigbourMetric {
    MANHATTAN
};

enum class RunningMode {
    SELECTION,
    SEPARATION
};

auto operator>>(std::istream& in, NeigbourMetric& num)
    -> std::istream&;
auto operator<<(std::ostream& os, const NeigbourMetric& num)
    -> std::ostream&;
auto operator>>(std::istream& in, RunningMode& num)
    -> std::istream&;
auto operator<<(std::ostream& os, const RunningMode& num)
    -> std::ostream&;

class ProgramOptions
{
public:
    ProgramOptions(std::string graph_file);

    auto getGraphFile() const
        -> std::string_view;

private:
    std::string graph_file_;
};

auto parseArguments(int argc, char* argv[])
    -> ProgramOptions;

} // namespace utils
