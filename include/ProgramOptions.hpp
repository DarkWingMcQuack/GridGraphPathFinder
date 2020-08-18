#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace util {

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

} // namespace util
