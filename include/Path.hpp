#pragma once

#include <GridGraph.hpp>
#include <functional>
#include <optional>
#include <string_view>
#include <vector>

namespace pathfinding {

using Distance = std::int64_t;
constexpr inline auto UNREACHABLE = std::numeric_limits<Distance>::max();

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
    friend auto operator<<(std::ostream& os, const Path& p)
        -> std::ostream&;

    std::vector<graph::Node> path_;
};

auto operator<<(std::ostream& os, const Path& p)
    -> std::ostream&;

} // namespace pathfinding
