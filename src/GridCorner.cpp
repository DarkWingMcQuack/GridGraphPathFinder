#include <GridCorner.hpp>
#include <iostream>
#include <cstdint>

using grid::GridCorner;


GridCorner::GridCorner(std::int64_t row,
                       std::int64_t column)
    : row_(row),
      column_(column) {}

auto GridCorner::operator==(const GridCorner& other) const noexcept
    -> bool
{
    return row_ == other.row_ && column_ == other.column_;
}

auto GridCorner::operator!=(const GridCorner& other) const noexcept
    -> bool
{
    return !(*this == other);
}


auto GridCorner::getRow() const noexcept
    -> std::int64_t
{
    return row_;
}

auto GridCorner::getColumn() const noexcept
    -> std::int64_t
{
    return column_;
}

auto grid::operator<<(std::ostream& os, const GridCorner& c) noexcept
    -> std::ostream&
{
    return os << "("
              << std::to_string(c.row_)
              << ", "
              << std::to_string(c.column_)
              << ")";
}
