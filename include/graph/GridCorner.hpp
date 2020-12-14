#pragma once

#include <cstdint>
#include <iostream>

namespace graph {

struct GridCorner
{
    GridCorner(std::int64_t row,
               std::int64_t column);
    GridCorner(GridCorner&&) = default;
    GridCorner(const GridCorner&) = default;

    auto operator=(GridCorner&&) -> GridCorner& = default;
    auto operator=(const GridCorner&) -> GridCorner& = default;


    auto operator==(const GridCorner& other) const noexcept
        -> bool;
    auto operator!=(const GridCorner& other) const noexcept
        -> bool;

    auto operator<(const GridCorner& other) const noexcept
        -> bool;

    auto getRow() const noexcept
        -> std::int32_t;
    auto getColumn() const noexcept
        -> std::int32_t;

    auto zScore() const noexcept
        -> std::uint64_t;


private:
    friend auto operator<<(std::ostream& os, const GridCorner& c) noexcept
        -> std::ostream&;

    friend class GridCell;

    std::int32_t row_;
    std::int32_t column_;
};

auto operator<<(std::ostream& os, const GridCorner& c) noexcept
    -> std::ostream&;

} // namespace graph
