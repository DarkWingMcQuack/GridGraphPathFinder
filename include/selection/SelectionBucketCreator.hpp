#pragma once

#include <fmt/core.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <pathfinding/Distance.hpp>
#include <selection/NodeSelection.hpp>
#include <selection/SelectionBucket.hpp>
#include <selection/SelectionBucketLookup.hpp>
#include <selection/SelectionLookup.hpp>
#include <vector>

namespace selection {

class SelectionBucketCreator
{
public:
    SelectionBucketCreator(const graph::GridGraph& graph,
                           std::vector<NodeSelection> selections,
                           std::vector<std::vector<std::size_t>> left_selections,
                           std::vector<std::vector<std::size_t>> right_selections);

    SelectionBucketCreator(SelectionLookup&& lookup);

    auto createBucketLookup() && noexcept
        -> SelectionBucketLookup;

private:
    auto getIncompleteNodeIdxLeft() const noexcept
        -> std::optional<std::size_t>;

    auto getIncompleteNodeIdxRight() const noexcept
        -> std::optional<std::size_t>;

    auto addBucketLeft(SelectionBucket bucket) noexcept
        -> void;

    auto addBucketRight(SelectionBucket bucket) noexcept
        -> void;

    auto bucketViableForLeft(const SelectionBucket& bucket,
                             std::size_t node_idx) const noexcept
        -> bool;

    auto bucketViableForRight(const SelectionBucket& bucket,
                              std::size_t node_idx) const noexcept
        -> bool;

    auto buildBuckedContainingAllOfNodeLeft(std::size_t idx) const noexcept
        -> SelectionBucket;

    auto buildBuckedContainingAllOfNodeRight(std::size_t idx) const noexcept
        -> SelectionBucket;

    auto countNodesAbleToUseBucketWithoutLeft(SelectionBucket bucket,
                                              const std::vector<std::size_t>& indices) const noexcept
        -> std::size_t;

    auto countNodesAbleToUseBucketWithoutRight(SelectionBucket bucket,
                                               const std::vector<std::size_t>& indices) const noexcept
        -> std::size_t;

    auto countNodesAbleToUseBucketLeft(const SelectionBucket& bucket) const noexcept
        -> std::size_t;

    auto countNodesAbleToUseBucketRight(const SelectionBucket& bucket) const noexcept
        -> std::size_t;

    auto countNodesUsingLeft(std::size_t selection_idx) const noexcept
        -> std::size_t;

    auto countNodesUsingRight(std::size_t selection_idx) const noexcept
        -> std::size_t;

    auto isCompleteLeft(std::size_t idx) const noexcept
        -> bool;

    auto isCompleteRight(std::size_t idx) const noexcept
        -> bool;

    auto optimizeBucketLeft(SelectionBucket bucket) const noexcept
        -> SelectionBucket;

    auto optimizeBucketRight(SelectionBucket bucket) const noexcept
        -> SelectionBucket;

    auto calculateConflictingSelections(const SelectionBucket& bucket,
                                        const std::vector<std::size_t>& selection_indices) const noexcept
        -> std::vector<std::size_t>;

    auto pruneSelectionBuckets() noexcept
        -> void;



private:
    const graph::GridGraph& graph_;
    std::vector<SelectionBucket> buckets_;
    std::vector<std::vector<std::size_t>> bucket_lookup_;
    std::vector<std::vector<std::size_t>> left_buckets_;
    std::vector<std::vector<std::size_t>> right_buckets_;

    std::vector<NodeSelection> selections_;
    std::vector<std::vector<std::size_t>> left_selections_;
    std::vector<std::vector<std::size_t>> right_selections_;

    std::vector<std::size_t> incomplete_nodes_left_;
    std::vector<std::size_t> incomplete_nodes_right_;
};

} // namespace selection
