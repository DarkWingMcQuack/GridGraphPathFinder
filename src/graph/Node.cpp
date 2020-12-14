#include <algorithm>
#include <fmt/core.h>
#include <fstream>
#include <graph/Node.hpp>
#include <vector>

using graph::Node;

auto Node::operator==(const Node& other) const noexcept
    -> bool
{
    return row == other.row && column == other.column;
}
auto Node::operator!=(const Node& other) const noexcept
    -> bool
{
    return !(*this == other);
}

auto Node::zScore() const noexcept
    -> std::uint64_t
{
    static constexpr std::array<uint32_t, 4> masks{0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};
    static constexpr std::array<uint32_t, 4> shifts{1, 2, 4, 8};

    uint64_t x = static_cast<uint32_t>(column); // Interleave lower 16 bits of x and y, so the bits of x
    uint64_t y = static_cast<uint32_t>(row); // are in the even positions and bits from y in the odd;

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

auto Node::operator<(const Node& other) const noexcept
    -> bool
{
    return zScore() < other.zScore();
}

auto Node::operator>(const Node& other) const noexcept
    -> bool
{
    return !(*this <= other);
}
auto Node::operator<=(const Node& other) const noexcept
    -> bool
{
    return (*this < other) || *this == other;
}
auto Node::operator>=(const Node& other) const noexcept
    -> bool
{
    return (*this > other) || *this == other;
}

auto Node::isManhattanNeigbourOf(const Node& other) const noexcept
    -> bool
{
    return other == Node{row, column + 1}
    or other == Node{row, column - 1}
    or other == Node{row - 1, column}
    or other == Node{row + 1, column};
}

auto graph::operator<<(std::ostream& os, Node n) noexcept
    -> std::ostream&
{
    return os << "Node{"
              << std::to_string(n.row)
              << ", "
              << std::to_string(n.column)
              << "}";
}
