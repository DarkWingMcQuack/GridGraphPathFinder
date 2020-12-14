#include <array>
#include <cstdint>
#include <graph/GridCorner.hpp>
#include <iostream>

using graph::GridCorner;


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

auto GridCorner::operator<(const GridCorner& other) const noexcept
    -> bool
{
    return zScore() < other.zScore();
}


auto GridCorner::getRow() const noexcept
    -> std::int32_t
{
    return row_;
}

auto GridCorner::getColumn() const noexcept
    -> std::int32_t
{
    return column_;
}

auto GridCorner::zScore() const noexcept
    -> std::uint64_t
{
    static constexpr std::array<uint32_t, 4> masks{0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};
    static constexpr std::array<uint32_t, 4> shifts{1, 2, 4, 8};

    uint64_t x = static_cast<uint32_t>(column_); // Interleave lower 16 bits of x and y, so the bits of x
    uint64_t y = static_cast<uint32_t>(row_); // are in the even positions and bits from y in the odd;

    x = (x | (x << shifts[3])) & masks[3];
    x = (x | (x << shifts[2])) & masks[2];
    x = (x | (x << shifts[1])) & masks[1];
    x = (x | (x << shifts[0])) & masks[0];

    y = (y | (y << shifts[3])) & masks[3];
    y = (y | (y << shifts[2])) & masks[2];
    y = (y | (y << shifts[1])) & masks[1];
    y = (y | (y << shifts[0])) & masks[0];

    const uint64_t result = x | (y << 1);

    return result;
}

auto graph::operator<<(std::ostream& os, const GridCorner& c) noexcept
    -> std::ostream&
{
    return os << "("
              << std::to_string(c.row_)
              << ", "
              << std::to_string(c.column_)
              << ")";
}
