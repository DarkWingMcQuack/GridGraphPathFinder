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
};

auto operator<<(std::ostream& os, Node n) noexcept
    -> std::ostream&;
} // namespace graph
