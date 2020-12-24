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

constexpr static inline auto PRETTY_PRINT = true;

class ProgramOptions
{
public:
    ProgramOptions(std::string graph_file,
                   NeigbourMetric neigbour_mode,
                   RunningMode running_mode,
                   std::optional<std::string> separation_folder = std::nullopt);

    auto getGraphFile() const noexcept
        -> std::string_view;

    auto getNeigbourCalculator() const noexcept
        -> graph::NeigbourCalculator;

    auto getRunningMode() const noexcept
        -> RunningMode;

    auto hasSeparationFolder() const noexcept
        -> bool;

    auto getSeparationFolder() const noexcept
        -> std::string_view;

private:
    std::string graph_file_;
    NeigbourMetric neigbour_mode_;
    RunningMode running_mode_;
    std::optional<std::string> separation_folder_;
};

auto parseArguments(int argc, char* argv[])
    -> ProgramOptions;

} // namespace utils
