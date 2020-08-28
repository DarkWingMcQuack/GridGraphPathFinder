#include <Node.hpp>
#include <algorithm>
#include <fmt/core.h>
#include <fstream>
#include <vector>

using graph::Node;

auto Node::operator==(const Node& other) const
    -> bool
{
    return row == other.row && column == other.column;
}
auto Node::operator!=(const Node& other) const
    -> bool
{
    return !(*this == other);
}

auto Node::operator<(const Node& other) const -> bool
{
    return row < other.row || column < other.column;
}
auto Node::operator>(const Node& other) const -> bool
{
    return !(*this <= other);
}
auto Node::operator<=(const Node& other) const -> bool
{
    return (*this < other) || *this == other;
}
auto Node::operator>=(const Node& other) const -> bool
{
    return (*this > other) || *this == other;
}


auto graph::operator<<(std::ostream& os, Node n)
    -> std::ostream&
{
    return os << "Node{" + std::to_string(n.row) + ", " + std::to_string(n.column) + "}";
}
