#pragma once

#include <graph/GridCell.hpp>
#include <graph/GridGraphIterator.hpp>
#include <graph/Node.hpp>
#include <optional>
#include <string_view>
#include <vector>

namespace graph {

class GridGraph
{
public:
    GridGraph(std::vector<std::vector<bool>> grid) noexcept;

    //the big 5
    GridGraph() = delete;
    GridGraph(GridGraph&&) = default;
    GridGraph(const GridGraph&) = delete;
    auto operator=(GridGraph &&) -> GridGraph& = delete;
    auto operator=(const GridGraph&) -> GridGraph& = delete;

    [[nodiscard]] auto isBarrier(const Node& n) const noexcept
        -> bool;

    [[nodiscard]] auto isWalkableNode(const Node& n) const noexcept
        -> bool;

    [[nodiscard]] auto getWalkableNeigbours(const Node& n) const noexcept
        -> std::vector<Node>;

    [[nodiscard]] auto getWalkableManhattanNeigbours(const Node& n) const noexcept
        -> std::vector<Node>;

    [[nodiscard]] auto getAllWalkableNodesOfCell(const graph::GridCell& cell) const noexcept
        -> std::vector<Node>;

    [[nodiscard]] auto generateRandomCellOfSize(std::int64_t cell_size) const noexcept
        -> graph::GridCell;

    [[nodiscard]] auto hasWalkableNode(const graph::GridCell& cell) const noexcept
        -> bool;

    [[nodiscard]] auto wrapGraphInCell() const noexcept
        -> graph::GridCell;

    [[nodiscard]] auto countNumberOfWalkableNodes(const graph::GridCell& cell) const noexcept
        -> std::size_t;

    [[nodiscard]] auto getRandomWalkableNode() const noexcept
        -> Node;

    auto begin() const noexcept
        -> GridGraphIterator;

    auto end() const noexcept
        -> GridGraphIterator;

    const std::size_t height;
    const std::size_t width;

private:
    std::vector<bool> grid_;
};

[[nodiscard]] auto parseFileToGridGraph(std::string_view path) noexcept
    -> std::optional<GridGraph>;

} // namespace graph
