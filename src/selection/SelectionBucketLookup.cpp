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
                                             std::vector<std::vector<std::size_t>> left_buckets,
                                             std::vector<std::vector<std::size_t>> right_buckets)
    : graph_(graph),
      buckets_(std::move(buckets)),
      left_buckets_(std::move(left_buckets)),
      right_buckets_(std::move(right_buckets)) {}
