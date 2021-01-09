#pragma once

#include <graph/GridCell.hpp>
#include <graph/GridGraphIterator.hpp>
#include <graph/NeigbourCalculator.hpp>
#include <graph/Node.hpp>
#include <optional>
#include <selection/NodeSelection.hpp>
#include <separation/Separation.hpp>
#include <string_view>
#include <vector>

namespace graph {

class GridGraph
{
public:
    GridGraph(std::vector<std::vector<bool>> grid,
              NeigbourCalculator neigbour_calculator) noexcept;

    //the big 5
    GridGraph() = delete;
    GridGraph(GridGraph&&) = default;
    GridGraph(const GridGraph&) = delete;
    auto operator=(GridGraph&&) -> GridGraph& = delete;
    auto operator=(const GridGraph&) -> GridGraph& = delete;

    [[nodiscard]] auto isBarrier(Node n) const noexcept
        -> bool;

    [[nodiscard]] auto isWalkableNode(Node n) const noexcept
        -> bool;

    [[nodiscard]] auto getWalkableNeigbours(Node n) const noexcept
        -> std::vector<Node>;

    [[nodiscard]] auto getAllWalkableNodesOfCell(const graph::GridCell& cell) const noexcept
        -> std::vector<Node>;

    [[nodiscard]] auto generateRandomCellOfSize(std::int64_t cell_size) const noexcept
        -> graph::GridCell;

    [[nodiscard]] auto hasWalkableNode(const graph::GridCell& cell) const noexcept
        -> bool;

    [[nodiscard]] auto areNeighbours(Node first, Node second) const noexcept
        -> bool;

    [[nodiscard]] auto getManhattanNeigbours(Node node) const noexcept
        -> std::array<Node, 4>;

    [[nodiscard]] auto getNeigboursOf(Node node) const noexcept
        -> std::vector<std::pair<Node, Distance>>;

    [[nodiscard]] auto hasBarrier(const graph::GridCell& cell) const noexcept
        -> bool;

    [[nodiscard]] auto hasBarrierNeigbour(Node n) const noexcept
        -> bool;

    [[nodiscard]] auto nodeToIndex(const graph::Node& n) const noexcept
        -> std::size_t;

    [[nodiscard]] auto indexToNode(std::size_t idx) const noexcept
        -> Node;

    [[nodiscard]] auto wrapGraphInCell() const noexcept
        -> graph::GridCell;

    [[nodiscard]] auto getAllCellsContaining(Node node) const noexcept
        -> std::vector<graph::GridCell>;

    [[nodiscard]] auto getAllParrentCells(GridCell cell) const noexcept
        -> std::vector<GridCell>;

    // returns two vectors of cells which when used in separations would have a better weight than
    // the separation between the given two cells
    // cells which contain both given cells are also not in the vectors
    [[nodiscard]] auto getAllPossibleSeparationCells(GridCell left, GridCell right) const noexcept
        -> std::pair<std::vector<GridCell>,
                     std::vector<GridCell>>;

    [[nodiscard]] auto countNumberOfWalkableNodes(const graph::GridCell& cell) const noexcept
        -> std::size_t;

    [[nodiscard]] auto getRandomWalkableNode() const noexcept
        -> Node;

    [[nodiscard]] auto countWalkableNodes() const noexcept
        -> std::size_t;

    [[nodiscard]] auto toClipped(Node n) const noexcept
        -> Node;

    [[nodiscard]] auto unclip(Node n) const noexcept
        -> Node;

    [[nodiscard]] auto toClipped(GridCell g) const noexcept
        -> GridCell;

    [[nodiscard]] auto unclip(GridCell g) const noexcept
        -> GridCell;

    [[nodiscard]] auto toClipped(GridCorner g) const noexcept
        -> GridCorner;

    [[nodiscard]] auto unclip(GridCorner g) const noexcept
        -> GridCorner;

    [[nodiscard]] auto unclip(separation::Separation g) const noexcept
        -> separation::Separation;

    [[nodiscard]] auto toClipped(separation::Separation g) const noexcept
        -> separation::Separation;

    [[nodiscard]] auto unclip(selection::NodeSelection g) const noexcept
        -> selection::NodeSelection;

    [[nodiscard]] auto toClipped(selection::NodeSelection g) const noexcept
        -> selection::NodeSelection;

    [[nodiscard]] auto getHeight() const noexcept
        -> std::size_t;

    [[nodiscard]] auto getWidth() const noexcept
        -> std::size_t;

    [[nodiscard]] auto getTrivialDistance(const Node& from, const Node& to) const noexcept
        -> Distance;

    [[nodiscard]] auto begin() const noexcept
        -> GridGraphIterator;

    [[nodiscard]] auto end() const noexcept
        -> GridGraphIterator;

    [[nodiscard]] auto size() const noexcept
        -> std::size_t;




private:
    std::vector<bool> grid_;
    NeigbourCalculator neigbour_calculator_;
    std::size_t height_;
    std::size_t width_;
    std::size_t clipped_height_ = 0;
    std::size_t clipped_width_ = 0;
};

[[nodiscard]] auto parseFileToGridGraph(std::string_view path,
                                        NeigbourCalculator neigbour_calc) noexcept
    -> std::optional<GridGraph>;

} // namespace graph
