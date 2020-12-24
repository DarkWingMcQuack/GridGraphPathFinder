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
    auto optimize(std::size_t idx) noexcept
        -> void;

    auto optimizeLeft(std::size_t idx) noexcept
        -> void;

    auto optimizeRight(std::size_t idx) noexcept
        -> void;

    auto getLeftOptimalGreedySelection(std::size_t node_idx,
                                       const std::unordered_set<graph::Node>& nodes) const noexcept
        -> std::size_t;

    auto getRightOptimalGreedySelection(std::size_t node_idx,
                                        const std::unordered_set<graph::Node>& nodes) const noexcept
        -> std::size_t;

private:
    const graph::GridGraph& graph_;

    std::vector<NodeSelection> selections_;

    std::vector<std::vector<std::size_t>> left_selections_;
    std::vector<std::vector<std::size_t>> right_selections_;

    std::unordered_set<std::size_t> keep_list_left_;
    std::unordered_set<std::size_t> keep_list_right_;
};

} // namespace selection
