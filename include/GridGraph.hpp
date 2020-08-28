#pragma once

#include <optional>
#include <string_view>
#include <vector>
#include <Node.hpp>

namespace graph {

class GridGraph
{
public:
    GridGraph(std::vector<std::vector<bool>> grid);

    //the big 5
    GridGraph() = delete;
    GridGraph(GridGraph&&) = default;
    GridGraph(const GridGraph&) = delete;
    auto operator=(GridGraph &&) -> GridGraph& = delete;
    auto operator=(const GridGraph&) -> GridGraph& = delete;

    //https://godbolt.org/z/zx8M6K
    auto isBarrier(const Node& n) const
        -> bool;

    auto isWalkableNode(const Node& n) const
        -> bool;

    auto getWalkableNeigbours(const Node& n) const
        -> std::vector<Node>;

    const std::size_t height;
    const std::size_t width;

private:
    std::vector<bool> grid_;
};

auto parseFileToGridGraph(std::string_view path)
    -> std::optional<GridGraph>;

} // namespace graph
