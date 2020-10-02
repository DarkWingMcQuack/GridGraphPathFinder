#include <graph/NeigbourCalculator.hpp>
#include <graph/Node.hpp>
#include <variant>
#include <vector>

using graph::ManhattanNeigbourCalculator;
using graph::AllSouroundingNeigbourCalculator;

auto ManhattanNeigbourCalculator::calculateNeigbours(const Node& node) const noexcept
    -> std::vector<Node>
{
    return {
        Node{node.row, node.column + 1},
        Node{node.row, node.column - 1},
        Node{node.row - 1, node.column},
        Node{node.row + 1, node.column},
    };
}

auto ManhattanNeigbourCalculator::isNeigbourOf(const Node& first, const Node& second) const noexcept
    -> bool
{
    return second == Node{first.row, first.column + 1}
    or second == Node{first.row, first.column - 1}
    or second == Node{first.row - 1, first.column}
    or second == Node{first.row + 1, first.column};
}

auto AllSouroundingNeigbourCalculator::calculateNeigbours(const Node& node) const noexcept
    -> std::vector<Node>
{
    return {
        Node{node.row, node.column + 1},
        Node{node.row, node.column - 1},
        Node{node.row - 1, node.column},
        Node{node.row + 1, node.column},
        Node{node.row + 1, node.column + 1},
        Node{node.row + 1, node.column - 1},
        Node{node.row - 1, node.column - 1},
        Node{node.row - 1, node.column + 1},
    };
}

auto AllSouroundingNeigbourCalculator::isNeigbourOf(const Node& first, const Node& second) const noexcept
    -> bool
{
    return second == Node{first.row, first.column + 1}
    or second == Node{first.row, first.column - 1}
    or second == Node{first.row - 1, first.column}
    or second == Node{first.row + 1, first.column}
    or second == Node{first.row + 1, first.column + 1}
    or second == Node{first.row + 1, first.column - 1}
    or second == Node{first.row - 1, first.column - 1}
    or second == Node{first.row - 1, first.column + 1};
}

auto graph::calculateNeigbours(const NeigbourCalculator& calculator,
                               const Node& node) noexcept
    -> std::vector<Node>
{
    return std::visit(
        [&](const auto& calc) {
            return calc.calculateNeigbours(node);
        },
        calculator);
}

auto graph::isNeigbourOf(const NeigbourCalculator& calculator,
                         const Node& first,
                         const Node& second) noexcept
    -> bool
{
    return std::visit(
        [&](const auto& calc) {
            return calc.isNeigbourOf(first, second);
        },
        calculator);
}
