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
                           std::vector<utils::RefVec<NodeSelection>> selections_per_node);

    SelectionBucketCreator(SelectionLookup&& lookup);

    auto createBucketLookup() && noexcept
        -> SelectionBucketLookup;

private:
    auto getIncompleteNodeIdx() const noexcept
        -> std::optional<std::size_t>;

    auto getIncompleteNeigboursOf(std::size_t idx) const noexcept
        -> std::vector<std::size_t>;

    auto addBucket(SelectionBucket bucket) noexcept
        -> void;

    auto buildBuckedContainingAllOfNode(std::size_t idx) const noexcept
        -> SelectionBucket;

    auto countNodesAbleToUseBucketWith(const SelectionBucket& bucket,
                                       const NodeSelection& selection) const noexcept
        -> std::size_t;

    auto countNodesAbleToUseBucketWithout(SelectionBucket bucket,
                                          const std::vector<NodeSelection>& selections) const noexcept
        -> std::size_t;

    auto countNodesAbleToUseBucket(const SelectionBucket& bucket) const noexcept
        -> std::size_t;

    auto countNodesUsing(const NodeSelection& selection) const noexcept
        -> std::size_t;

    auto isComplete(std::size_t idx) const noexcept
        -> bool;

    auto optimizeBucket(SelectionBucket bucket) const noexcept
        -> SelectionBucket;

    auto calculateConflictingSelections(const SelectionBucket& bucket,
                                        const utils::RefVec<NodeSelection>& selections) const noexcept
        -> std::vector<NodeSelection>;

    auto pruneSelectionBuckets() noexcept
        -> void;



private:
    const graph::GridGraph& graph_;
    std::vector<SelectionBucket> buckets_;
    std::vector<std::vector<std::size_t>> bucket_lookup_;

    std::vector<NodeSelection> selections_;
    std::vector<utils::RefVec<NodeSelection>> selections_per_node_;
    std::vector<std::size_t> incomplete_nodes_;
};

} // namespace selection
