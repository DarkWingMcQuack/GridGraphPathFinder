#include <fmt/core.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <pathfinding/Distance.hpp>
#include <selection/NodeSelection.hpp>
#include <selection/SelectionBucket.hpp>
#include <selection/SelectionBucketCreator.hpp>
#include <utils/Range.hpp>
#include <vector>

using selection::SelectionBucket;
using selection::SelectionBucketCreator;
using selection::NodeSelection;
using graph::GridGraph;


SelectionBucketCreator::SelectionBucketCreator(const graph::GridGraph& graph,
                                               std::vector<NodeSelection> selections,
                                               std::vector<std::vector<NodeSelection*>> selections_per_node)
    : graph_(graph),
      selections_(std::move(selections)),
      selections_per_node_(std::move(selections_per_node)) {}

auto SelectionBucketCreator::getIncompleteNodeIdx() const noexcept
    -> std::optional<std::size_t>
{
    for(std::size_t i{0}; i < selections_per_node_.size(); i++) {
        if(!selections_per_node_[i].empty()) {
            return i;
        }
    }

    return std::nullopt;
}

auto SelectionBucketCreator::addBucket(SelectionBucket bucket) noexcept
    -> void
{
    buckets_.emplace_back(std::move(bucket));
    auto& bucket_ref = buckets_.back();

    // TODO: do in parallel
    for(std::size_t i{0}; i < selections_per_node_.size(); i++) {
        auto& selections = selections_per_node_[i];

        if(bucket_ref.isSuperSetOf(selections)) {
            selections.erase(
                std::remove_if(std::begin(selections),
                               std::end(selections),
                               [&](const auto* selection) {
                                   return bucket_ref.contains(*selection);
                               }),
                std::end(selections));

            bucket_lookup_[i].emplace_back(&bucket_ref);
        }
    }
}

auto SelectionBucketCreator::countNodesAbleToUseBucketWith(const SelectionBucket& bucket,
                                                           const NodeSelection& selection) const noexcept
    -> std::size_t
{
    std::vector<std::size_t> indices;
    const auto range = utils::range(selections_per_node_.size());
    std::copy_if(std::begin(range),
                 std::end(range),
                 std::back_inserter(indices),
                 [&](auto idx) {
                     const auto& selections = selections_per_node_[idx];
                     return bucket.isSuperSetOf(selections);
                 });

    indices.erase(
        std::remove_if(std::begin(indices),
                       std::end(indices),
                       [&](auto idx) {
                           const auto& selections = selections_per_node_[idx];
                           return std::none_of(std::begin(selections),
                                               std::end(selections),
                                               [&](const auto* selct) {
                                                   return *selct == selection;
                                               });
                       }),
        std::end(indices));

    return indices.size();
}

auto SelectionBucketCreator::countNodesAbleToUseBucket(const SelectionBucket& bucket) const noexcept
    -> std::size_t
{
    return std::count_if(std::begin(selections_per_node_),
                         std::end(selections_per_node_),
                         [&](const auto selections) {
                             return bucket.isSuperSetOf(selections);
                         });
}

auto SelectionBucketCreator::countNodesUsing(const NodeSelection& selection) const noexcept
    -> std::size_t
{
    return std::count_if(std::begin(selections_per_node_),
                         std::end(selections_per_node_),
                         [&](const auto& selections) {
                             return std::find_if(std::begin(selections),
                                                 std::end(selections),
                                                 [&](const auto* sel) {
                                                     return selection == *sel;
                                                 })
                                 != std::end(selections);
                         });
}
