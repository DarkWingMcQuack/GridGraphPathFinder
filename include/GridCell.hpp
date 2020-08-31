#pragma once

#include <GridCellIterator.hpp>
#include <Node.hpp>
#include <array>
#include <iostream>

namespace grid {

struct GridCorner
{
    GridCorner(std::int64_t row,
               std::int64_t column);
    GridCorner(GridCorner&&) = default;
    GridCorner(const GridCorner&) = default;

    auto operator=(GridCorner &&) -> GridCorner& = default;
    auto operator=(const GridCorner&) -> GridCorner& = default;


    auto operator==(const GridCorner& other) const noexcept
        -> bool;
    auto operator!=(const GridCorner& other) const noexcept
        -> bool;

    auto getRow() const noexcept
        -> std::int64_t;
    auto getColumn() const noexcept
        -> std::int64_t;


private:
    friend auto operator<<(std::ostream& os, const GridCorner& c) noexcept
        -> std::ostream&;

    friend class GridCell;

    std::int64_t row_;
    std::int64_t column_;
};

auto operator<<(std::ostream& os, const GridCorner& c) noexcept
    -> std::ostream&;

class GridCell
{
public:
    GridCell(GridCorner top_left,
             GridCorner top_right,
             GridCorner bottom_left,
             GridCorner bottom_right) noexcept;

    GridCell(GridCell&&) noexcept = default;
    GridCell(const GridCell&) noexcept = default;

    auto operator=(GridCell&&) noexcept
        -> GridCell& = default;
    auto operator=(const GridCell&) noexcept
        -> GridCell& = default;

    auto operator==(const GridCell& other) const noexcept
        -> bool;
    auto operator!=(const GridCell& other) const noexcept
        -> bool;

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

    [[nodiscard]] auto getHeight() const noexcept
        -> std::size_t;

    auto getNodeAt(std::size_t row, std::size_t column) const noexcept
        -> std::optional<graph::Node>;

    auto begin() const noexcept
        -> GridCellIterator;

    auto end() const noexcept
        -> GridCellIterator;

private:
    friend auto operator<<(std::ostream& os, const GridCell& c) noexcept
        -> std::ostream&;

    GridCorner top_left_;
    GridCorner top_right_;
    GridCorner bottom_left_;
    GridCorner bottom_right_;
};

[[nodiscard]] auto merge(const GridCell& first,
                         const GridCell& second,
                         const GridCell& third,
                         const GridCell& fourth) noexcept
    -> GridCell;

auto operator<<(std::ostream& os, const GridCell& c) noexcept
    -> std::ostream&;

} // namespace grid