#pragma once

#include <optional>
#include <string_view>
#include <vector>

namespace graph {

struct Node
{
    std::size_t row;
    std::size_t column;

    auto operator==(const Node& other) const -> bool;
    auto operator!=(const Node& other) const -> bool;
    auto operator<(const Node& other) const -> bool;
    auto operator>(const Node& other) const -> bool;
    auto operator<=(const Node& other) const -> bool;
    auto operator>=(const Node& other) const -> bool;
};

auto operator<<(std::ostream& os, Node n)
    -> std::ostream&;
} // namespace graph
