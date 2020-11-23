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

auto Node::operator<(const Node& other) const noexcept
    -> bool
{
    //weiter oben?
    if(row < other.row) {
        return true;
    }

	//weiter unten?
    if(row > other.row) {
        return false;
    }

	//gleiche row, dann vergleiche column
    return column < other.column;
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
