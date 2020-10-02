#pragma once

#include <graph/NeigbourCalculator.hpp>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

namespace utils {

enum class NeigbourMetric {
    MANHATTAN,
    ALL_SURROUNDING
};

enum class RunningMode {
    SELECTION,
    SEPARATION
};

class ProgramOptions
{
public:
    ProgramOptions(std::string graph_file,
                   NeigbourMetric neigbour_mode,
                   RunningMode running_mode);

    auto getGraphFile() const noexcept
        -> std::string_view;

    auto getNeigbourCalculator() const noexcept
        -> graph::NeigbourCalculator;

    auto getRunningMode() const noexcept
        -> RunningMode;

private:
    std::string graph_file_;
    NeigbourMetric neigbour_mode_;
    RunningMode running_mode_;
};

auto parseArguments(int argc, char* argv[])
    -> ProgramOptions;

} // namespace utils
