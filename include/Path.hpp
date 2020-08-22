#pragma once

#include <GridGraph.hpp>
#include <functional>
#include <optional>
#include <string_view>
#include <vector>

namespace pathfinding {

class Path
{
public:
    Path(std::vector<graph::Node> path = {});
    Path(Path&&) = default;
    Path(const Path&) = default;

    auto operator=(Path &&) -> Path& = default;
    auto operator=(const Path&) -> Path& = default;

    auto pushBack(graph::Node node) -> void;
    auto pushFront(graph::Node node) -> void;

    auto getLength() const -> std::size_t;

    auto getSource() const -> const graph::Node&;
    auto getSource() -> graph::Node&;

    auto getTarget() const -> const graph::Node&;
    auto getTarget() -> graph::Node&;

private:
    std::vector<graph::Node> path_;
};

} // namespace pathfinding
