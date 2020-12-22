#pragma once

#include <fmt/core.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <pathfinding/Distance.hpp>
#include <queue>
#include <random>
#include <selection/NodeSelection.hpp>
#include <selection/NodeSelectionCalculator.hpp>
#include <selection/SelectionLookup.hpp>
#include <unordered_set>
#include <vector>

namespace selection {

class SelectionLookupOptimizer
{
public:
    SelectionLookupOptimizer(SelectionLookup&& lookup);

    auto optimize() noexcept
        -> void;

    auto getLookup() && noexcept
        -> SelectionLookup;

private:
    auto optimize(std::size_t idx) const noexcept
        -> void;

    auto optimizeLeft(std::size_t idx) const noexcept
        -> void;

    auto optimizeRight(std::size_t idx) const noexcept
        -> void;

private:
    const graph::GridGraph& graph_;
    std::vector<NodeSelection> selections_;

    std::vector<std::vector<std::size_t>> left_selections_;
    std::vector<std::vector<std::size_t>> right_selections_;
};

} // namespace selection
