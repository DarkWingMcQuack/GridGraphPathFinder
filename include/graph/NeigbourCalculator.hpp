#pragma once

#include <graph/Node.hpp>
#include <pathfinding/Distance.hpp>
#include <variant>
#include <vector>

namespace graph {

class ManhattanNeigbourCalculator
{
public:
    auto calculateNeigbours(const Node& node) const noexcept
        -> std::vector<Node>;
    auto isNeigbourOf(const Node& first, const Node& second) const noexcept
        -> bool;
    auto getTrivialDistance(const Node& from, const Node& to) const noexcept
        -> Distance;
};

class AllSouroundingNeigbourCalculator
{
public:
    auto calculateNeigbours(const Node& node) const noexcept
        -> std::vector<Node>;
    auto isNeigbourOf(const Node& first, const Node& second) const noexcept
        -> bool;
    auto getTrivialDistance(const Node& from, const Node& to) const noexcept
        -> Distance;
};

using NeigbourCalculator = std::variant<ManhattanNeigbourCalculator,
                                        AllSouroundingNeigbourCalculator>;

auto calculateNeigbours(const NeigbourCalculator& calculator,
                        const Node& node) noexcept
    -> std::vector<Node>;

auto isNeigbourOf(const NeigbourCalculator& calculator,
                  const Node& first,
                  const Node& second) noexcept
    -> bool;

auto getTrivialDistance(const NeigbourCalculator& calculator,
                        const Node& first,
                        const Node& second) noexcept
    -> Distance;


} // namespace graph
