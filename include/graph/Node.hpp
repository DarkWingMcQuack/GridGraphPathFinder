#pragma once

#include <optional>
#include <string_view>
#include <vector>

namespace graph {

struct Node
{
    std::size_t row;
    std::size_t column;

    auto operator==(const Node& other) const noexcept -> bool;
    auto operator!=(const Node& other) const noexcept -> bool;
    auto operator<(const Node& other) const noexcept -> bool;
    auto operator>(const Node& other) const noexcept -> bool;
    auto operator<=(const Node& other) const noexcept -> bool;
    auto operator>=(const Node& other) const noexcept -> bool;

    auto isManhattanNeigbourOf(const Node& other) const noexcept
        -> bool;

    auto zScore() const noexcept
        -> std::uint64_t;
};

auto operator<<(std::ostream& os, Node n) noexcept
    -> std::ostream&;
} // namespace graph


// hashers to make it possible to use Nodes in unoredered_sets and unordered_maps
namespace std {

template<>
class hash<graph::Node>
{
public:
    auto operator()(const graph::Node& n) const noexcept
        -> std::size_t
    {
        return std::hash<std::size_t>()(n.row)
            xor std::hash<std::size_t>()(n.column);
    }
};

template<>
class hash<std::pair<graph::Node, graph::Node>>
{
public:
    auto operator()(const std::pair<graph::Node, graph::Node>& n) const noexcept
        -> std::size_t
    {
        return std::hash<graph::Node>()(n.first)
            xor std::hash<graph::Node>()(n.second);
    }
};


} // namespace std
