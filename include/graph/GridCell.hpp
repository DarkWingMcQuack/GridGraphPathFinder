#pragma once

#include <array>
#include <graph/GridCellIterator.hpp>
#include <graph/GridCorner.hpp>
#include <graph/Node.hpp>
#include <iostream>
#include <utils/Utils.hpp>

namespace graph {

enum class CellOrientation {
    HORIZONTAL,
    VERTICAL,
    OTHER
};

class GridCell
{
public:
    GridCell(GridCorner top_left,
             GridCorner bottom_right) noexcept;

    // wrap a single node in a gridcell
    GridCell(Node node) noexcept;

    GridCell(GridCell&&) noexcept = default;
    GridCell(const GridCell&) noexcept = default;

    auto operator=(GridCell&&) noexcept
        -> GridCell& = default;
    auto operator=(const GridCell&) noexcept
        -> GridCell& = default;

    [[nodiscard]] auto operator==(const GridCell& other) const noexcept
        -> bool;
    [[nodiscard]] auto operator!=(const GridCell& other) const noexcept
        -> bool;
    [[nodiscard]] auto operator<(const GridCell& other) const noexcept
        -> bool;

    [[nodiscard]] auto operator[](std::size_t idx) const noexcept
        -> graph::Node;

    [[nodiscard]] auto isInCell(const graph::Node& node) const noexcept
        -> bool;

    [[nodiscard]] auto isAtomic() const noexcept
        -> bool;

    [[nodiscard]] auto isValid() const noexcept
        -> bool;

    [[nodiscard]] auto isSplitable() const noexcept
        -> bool;

    [[nodiscard]] auto split() const noexcept
        -> std::array<GridCell, 4>;

    [[nodiscard]] auto getWidth() const noexcept
        -> std::size_t;

    [[nodiscard]] auto getZScoreRange() const noexcept
        -> std::pair<std::int64_t, std::int64_t>;

    [[nodiscard]] auto getHeight() const noexcept
        -> std::size_t;

    [[nodiscard]] auto getTopLeft() const noexcept
        -> GridCorner;
    [[nodiscard]] auto getTopRight() const noexcept
        -> GridCorner;
    [[nodiscard]] auto getBottomLeft() const noexcept
        -> GridCorner;
    [[nodiscard]] auto getBottomRight() const noexcept
        -> GridCorner;

    [[nodiscard]] auto size() const noexcept
        -> std::size_t;

    [[nodiscard]] auto cacluclateOrientation(const GridCell& other) const noexcept
        -> CellOrientation;

    [[nodiscard]] auto getNodeAt(std::size_t row, std::size_t column) const noexcept
        -> std::optional<graph::Node>;

    [[nodiscard]] auto begin() const noexcept
        -> GridCellIterator;

    [[nodiscard]] auto end() const noexcept
        -> GridCellIterator;

    [[nodiscard]] auto isSuperSetOf(const GridCell& other) const noexcept
        -> bool;

    [[nodiscard]] auto isSubSetOf(const GridCell& other) const noexcept
        -> bool;

    [[nodiscard]] auto hasCommonNodeWith(const GridCell& other) const noexcept
        -> bool;

    [[nodiscard]] auto to_string() const noexcept
        -> std::string;

    [[nodiscard]] static auto wrapInCell(Node n) noexcept
        -> GridCell;

private:
    friend auto operator<<(std::ostream& os, const GridCell& c) noexcept
        -> std::ostream&;


    friend auto merge(const GridCell& first,
                      const GridCell& second,
                      const GridCell& third,
                      const GridCell& fourth) noexcept
        -> GridCell;

    friend class CellRowWalker;
    friend class CellColumnWalker;

    GridCorner top_left_;
    GridCorner bottom_right_;
};

[[nodiscard]] auto merge(const GridCell& first,
                         const GridCell& second,
                         const GridCell& third,
                         const GridCell& fourth) noexcept
    -> GridCell;

auto operator<<(std::ostream& os, const GridCell& c) noexcept
    -> std::ostream&;

} // namespace graph


namespace std {

template<>
class hash<graph::GridCell>
{
public:
    auto operator()(const graph::GridCell& n) const noexcept
        -> std::size_t
    {
        return utils::hashCombine(n.getTopLeft().getRow(),
                                  n.getBottomRight().getRow(),
                                  n.getTopLeft().getColumn(),
                                  n.getBottomRight().getColumn());
    }
};

template<>
class hash<std::pair<graph::GridCell, graph::GridCell>>
{
public:
    auto operator()(const std::pair<graph::GridCell, graph::GridCell>& n) const noexcept
        -> std::size_t
    {
        return utils::hashCombine(n.first, n.second);
    }
};


} // namespace std
