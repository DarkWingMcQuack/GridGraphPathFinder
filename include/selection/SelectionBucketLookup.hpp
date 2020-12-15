#pragma once

#include <fmt/core.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <pathfinding/Distance.hpp>
#include <selection/NodeSelection.hpp>
#include <selection/SelectionBucket.hpp>
#include <vector>

namespace selection {

class SelectionBucketLookup
{
public:
    SelectionBucketLookup(const graph::GridGraph& graph,
                          std::vector<SelectionBucket> buckets,
                          std::vector<std::vector<std::size_t>> bucket_lookup);

private:
    const graph::GridGraph& graph_;
    std::vector<SelectionBucket> buckets_;
    std::vector<std::vector<std::size_t>> bucket_lookup_;
};

} // namespace selection
