#pragma once

#include <fmt/core.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <pathfinding/Distance.hpp>
#include <selection/NodeSelection.hpp>
#include <selection/SelectionBucket.hpp>
#include <selection/SelectionBucketLookup.hpp>
#include <vector>

namespace selection {

class SelectionBucketCreator
{
    SelectionBucketCreator(const graph::GridGraph& graph,
                           std::vector<NodeSelection> selections,
                           std::vector<std::vector<NodeSelection*>> selections_per_node);


    auto createBucketLookup() && noexcept
        -> SelectionBucketLookup;


private:
    auto getIncompleteNodeIdx() const noexcept
        -> std::optional<std::size_t>;

    auto addBucket(SelectionBucket bucket) noexcept
        -> void;

    auto countNodesAbleToUseBucketWith(const SelectionBucket& bucket,
                                       const NodeSelection& selection) const noexcept
        -> std::size_t;

    auto countNodesAbleToUseBucket(const SelectionBucket& bucket) const noexcept
        -> std::size_t;

    auto countNodesUsing(const NodeSelection& selection) const noexcept
        -> std::size_t;



private:
    const graph::GridGraph& graph_;
    std::vector<SelectionBucket> buckets_;
    std::vector<std::vector<SelectionBucket*>> bucket_lookup_;

    std::vector<NodeSelection> selections_;
    std::vector<std::vector<NodeSelection*>> selections_per_node_;
};

} // namespace selection
