#include <fmt/core.h>
#include <fmt/ostream.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <pathfinding/Distance.hpp>
#include <progresscpp/ProgressBar.hpp>
#include <selection/NodeSelection.hpp>
#include <selection/SelectionBucket.hpp>
#include <selection/SelectionBucketCreator.hpp>
#include <selection/SelectionLookupOptimizer.hpp>
#include <utils/Range.hpp>
#include <vector>

using selection::SelectionLookup;
using selection::SelectionLookupOptimizer;

SelectionLookupOptimizer::SelectionLookupOptimizer(SelectionLookup&& lookup)
    : graph_(lookup.graph_),
      selections_(std::move(lookup.selections_)),
      left_selections_(std::move(lookup.left_selections_)),
      right_selections_(std::move(lookup.right_selections_)) {}

auto SelectionLookupOptimizer::optimize() noexcept
    -> void
{
    auto size = graph_.countWalkableNodes();

    fmt::print("optimizing patch lookup...\n");
    progresscpp::ProgressBar bar{size, 80ul};

    for(auto n : graph_) {
        auto idx = graph_.nodeToIndex(n);
        optimize(idx);

        bar++;
        bar.displayIfChangedAtLeast(0.001);
    }

    bar.done();
}

auto SelectionLookupOptimizer::getLookup() && noexcept
    -> SelectionLookup
{
    return SelectionLookup{graph_,
                           std::move(selections_),
                           std::move(left_selections_),
                           std::move(right_selections_)};
}

auto SelectionLookupOptimizer::optimize(std::size_t idx) noexcept
    -> void
{
    optimizeLeft(idx);
    optimizeRight(idx);
}

auto SelectionLookupOptimizer::getLeftOptimalGreedySelection(std::size_t node_idx,
                                                             const std::unordered_set<graph::Node>& nodes) const noexcept
    -> std::size_t
{
    const auto& node_selects = left_selections_[node_idx];
    auto best_index = node_selects[0];
    auto best_score = 0;

    for(auto idx : node_selects) {
        const auto& right_nodes = selections_[idx].getRightSelection();
        auto score = std::count_if(std::begin(right_nodes),
                                   std::end(right_nodes),
                                   [&](auto node) {
                                       return nodes.count(node) == 0;
                                   });

        if(score > best_score) {
            best_score = score;
            best_index = idx;
        }
    }

    return best_index;
}

auto SelectionLookupOptimizer::getRightOptimalGreedySelection(std::size_t node_idx,
                                                              const std::unordered_set<graph::Node>& nodes) const noexcept
    -> std::size_t
{
    const auto& node_selects = right_selections_[node_idx];
    auto best_index = node_selects[0];
    auto best_score = 0;

    for(auto idx : node_selects) {
        const auto& left_nodes = selections_[idx].getLeftSelection();
        auto score = std::count_if(std::begin(left_nodes),
                                   std::end(left_nodes),
                                   [&](auto node) {
                                       return nodes.count(node) == 0;
                                   });

        if(score > best_score) {
            best_score = score;
            best_index = idx;
        }
    }

    return best_index;
}

auto SelectionLookupOptimizer::optimizeLeft(std::size_t node_idx) noexcept
    -> void
{
    const auto& left_secs = left_selections_[node_idx];
    std::unordered_set<graph::Node> all_nodes;

    for(auto idx : left_secs) {
        const auto& right_nodes = selections_[idx].getRightSelection();
        all_nodes.insert(std::begin(right_nodes),
                         std::end(right_nodes));
    }

    std::vector<std::size_t> new_selection_set;
    std::unordered_set<graph::Node> covered_nodes;
    for(auto idx : left_secs) {
        if(keep_list_left_.count(idx) == 0) {
            continue;
        }

        const auto& right_nodes = selections_[idx].getRightSelection();
        covered_nodes.insert(std::begin(right_nodes),
                             std::end(right_nodes));
        new_selection_set.emplace_back(idx);
    }

    while(covered_nodes.size() < all_nodes.size()) {
        auto next_selection_idx = getLeftOptimalGreedySelection(node_idx, covered_nodes);
        const auto& right_nodes = selections_[next_selection_idx].getRightSelection();
        covered_nodes.insert(std::begin(right_nodes),
                             std::end(right_nodes));
        new_selection_set.emplace_back(next_selection_idx);
        keep_list_left_.emplace(next_selection_idx);
    }

    left_selections_[node_idx] = std::move(new_selection_set);
}

auto SelectionLookupOptimizer::optimizeRight(std::size_t node_idx) noexcept
    -> void
{
    const auto& right_secs = right_selections_[node_idx];
    std::unordered_set<graph::Node> all_nodes;


    for(auto idx : right_secs) {
        const auto& left_nodes = selections_[idx].getLeftSelection();
        all_nodes.insert(std::begin(left_nodes),
                         std::end(left_nodes));
    }

    std::vector<std::size_t> new_selection_set;
    std::unordered_set<graph::Node> covered_nodes;
    for(auto idx : right_secs) {
        if(keep_list_right_.count(idx) == 0) {
            continue;
        }

        const auto& left_nodes = selections_[idx].getLeftSelection();
        covered_nodes.insert(std::begin(left_nodes),
                             std::end(left_nodes));
        new_selection_set.emplace_back(idx);
    }

    while(covered_nodes.size() < all_nodes.size()) {
        auto next_selection_idx = getRightOptimalGreedySelection(node_idx, covered_nodes);
        const auto& left_nodes = selections_[next_selection_idx].getLeftSelection();
        covered_nodes.insert(std::begin(left_nodes),
                             std::end(left_nodes));
        new_selection_set.emplace_back(next_selection_idx);
        keep_list_right_.emplace(next_selection_idx);
    }

    right_selections_[node_idx] = std::move(new_selection_set);
}
