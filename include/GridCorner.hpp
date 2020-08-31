#pragma once

#include <iostream>
#include <cstdint>

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

} // namespace grid
