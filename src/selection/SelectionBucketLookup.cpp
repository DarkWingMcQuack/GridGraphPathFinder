#include <fmt/core.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <pathfinding/Distance.hpp>
#include <selection/NodeSelection.hpp>
#include <selection/SelectionBucket.hpp>
#include <selection/SelectionBucketLookup.hpp>
#include <utility>
#include <vector>

using selection::SelectionBucket;
using selection::SelectionBucketLookup;

SelectionBucketLookup::SelectionBucketLookup(const graph::GridGraph& graph,
                                             std::vector<SelectionBucket> buckets,
                                             std::vector<std::vector<std::size_t>> bucket_lookup)
    : graph_(graph),
      buckets_(std::move(buckets)),
      bucket_lookup_(std::move(bucket_lookup)) {}
