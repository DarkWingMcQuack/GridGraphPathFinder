#include <execution>
#include <fmt/core.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <pathfinding/Distance.hpp>
#include <selection/NodeSelection.hpp>
#include <selection/SelectionBucket.hpp>
#include <selection/SelectionBucketCreator.hpp>
#include <selection/SelectionLookupOptimizer.hpp>
#include <utils/Range.hpp>
#include <vector>

using selection::SelectionLookup;
using selection::SelectionLookupOptimizer;
using graph::GridGraph;

SelectionLookupOptimizer::SelectionLookupOptimizer(SelectionLookup&& lookup)
    : graph_(lookup.graph_),
      selections_(std::move(lookup.selections_)),
      left_selections_(std::move(lookup.left_selections_)),
      right_selections_(std::move(lookup.right_selections_)) {}



auto SelectionLookupOptimizer::optimize() noexcept
    -> void
{
    std::for_each(std::execution::par,
                  std::begin(graph_),
                  std::end(graph_),
                  [&](auto node) {
                      const auto idx = graph_.nodeToIndex(node);
                      optimize(idx);
                  });
}

auto SelectionLookupOptimizer::getLookup() && noexcept
    -> SelectionLookup
{
}

auto SelectionLookupOptimizer::optimize(std::size_t idx) const noexcept
    -> void
{
}

auto SelectionLookupOptimizer::optimizeLeft(std::size_t idx) const noexcept
    -> void
{
    const auto& selection_indices = left_selections_[idx];

    utils::CRefVec<NodeSelection> left_selections;
    left_selections.reserve(selection_indices.size());
    std::transform(std::begin(selection_indices), std::end(selection_indices),
                   std::back_inserter(left_selections),
                   [&](auto index) {
                       return std::cref(selections_[index]);
                   });
}

auto SelectionLookupOptimizer::optimizeRight(std::size_t idx) const noexcept
    -> void
{}
