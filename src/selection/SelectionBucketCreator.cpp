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
                                               std::vector<utils::RefVec<NodeSelection>> selections_per_node)
    : graph_(graph),
      selections_(std::move(selections)),
      selections_per_node_(std::move(selections_per_node))
{
    for(auto& selections : selections_per_node_) {
        std::sort(std::begin(selections),
                  std::end(selections),
                  [](auto lhs, auto rhs) {
                      return lhs.get() < rhs.get();
                  });
    }
}

auto SelectionBucketCreator::getIncompleteNodeIdx() const noexcept
    -> std::optional<std::size_t>
{
    for(auto i : utils::range(selections_per_node_.size())) {
        if(!isComplete(i)) {
            return i;
        }
    }

    return std::nullopt;
}


auto SelectionBucketCreator::createBucketLookup() && noexcept
    -> SelectionBucketLookup
{
    while(auto current_idx_opt = getIncompleteNodeIdx()) {
        const auto current_idx = current_idx_opt.value();
        auto current_bucket = buildBuckedContainingAllOfNode(current_idx);
        current_bucket = optimizeBucket(std::move(current_bucket));

        addBucket(std::move(current_bucket));
    }

    return SelectionBucketLookup{graph_,
                                 std::move(buckets_),
                                 std::move(bucket_lookup_)};
}


auto SelectionBucketCreator::optimizeBucket(SelectionBucket bucket) const noexcept
    -> SelectionBucket
{
    auto changed = true;
    while(changed) {
        changed = false;
        auto best_ratio = 1.0;
        std::vector<NodeSelection> best_erase;
        for(auto node : graph_) {
            auto idx = graph_.nodeToIndex(node);

            if(isComplete(idx)) {
                continue;
            }

            const auto& selections = selections_per_node_[idx];
            auto conflicts = calculateConflictingSelections(bucket, selections);

            const auto number_of_conficts = conflicts.size();
            auto nodes_using_new_bucket = countNodesAbleToUseBucketWithout(bucket, conflicts);
            auto new_ratio = static_cast<double>(nodes_using_new_bucket)
                / static_cast<double>(number_of_conficts);

            if(best_ratio < new_ratio) {
                best_erase = std::move(conflicts);
                best_ratio = new_ratio;
                changed = true;
            }
        }

        for(const auto& s : best_erase) {
            bucket = std::move(bucket).exclude(s);
        }
    }

    return bucket;
}

auto SelectionBucketCreator::getIncompleteNeigboursOf(std::size_t idx) const noexcept
    -> std::vector<std::size_t>
{
    const auto node = graph_.indexToNode(idx);
    auto neigbours = graph_.getWalkableNeigbours(node);

    neigbours.erase(
        std::remove_if(std::begin(neigbours),
                       std::end(neigbours),
                       [&](auto n) {
                           const auto neig_idx = graph_.nodeToIndex(n);
                           return isComplete(neig_idx);
                       }),
        std::end(neigbours));

    std::vector<std::size_t> indices;
    std::transform(std::begin(neigbours),
                   std::end(neigbours),
                   std::back_inserter(indices),
                   [&](auto n) {
                       return graph_.nodeToIndex(n);
                   });

    return indices;
}

auto SelectionBucketCreator::isComplete(std::size_t idx) const noexcept
    -> bool
{
    return selections_per_node_[idx].empty();
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
                               [&](auto selection) {
                                   return bucket_ref.contains(selection);
                               }),
                std::end(selections));

            bucket_lookup_[i].emplace_back(std::ref(bucket_ref));
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
        std::remove_if(
            std::begin(indices),
            std::end(indices),
            [&](auto idx) {
                const auto& selections = selections_per_node_[idx];
                return std::binary_search(
                    std::begin(selections),
                    std::end(selections),
                    std::ref(selection),
                    [&](const auto& lhs, const auto& rhs) {
                        return lhs.get() < rhs.get();
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
                             return std::binary_search(std::begin(selections),
                                                       std::end(selections),
                                                       std::ref(selection),
                                                       [](const auto& lhs, const auto& rhs) {
                                                           return lhs.get() < rhs.get();
                                                       });
                         });
}

auto SelectionBucketCreator::buildBuckedContainingAllOfNode(std::size_t idx) const noexcept
    -> SelectionBucket
{
    const auto& node_selection = selections_per_node_[idx];
    std::vector<NodeSelection> bucket_selections;
    std::transform(std::begin(node_selection),
                   std::end(node_selection),
                   std::back_inserter(bucket_selections),
                   [](auto ptr) {
                       return ptr.get();
                   });

    return SelectionBucket{std::move(bucket_selections)};
}

auto SelectionBucketCreator::calculateConflictingSelections(const SelectionBucket& bucket,
                                                            const utils::RefVec<NodeSelection>& selections) const noexcept
    -> std::vector<NodeSelection>
{
    const auto& bucket_selections = bucket.getSelections();

    std::vector<NodeSelection> ret_vec;
    std::copy_if(std::begin(bucket_selections),
                 std::end(bucket_selections),
                 std::back_inserter(ret_vec),
                 [&](const auto& selection) {
                     return std::binary_search(std::begin(selections),
                                               std::end(selections),
                                               std::ref(selection),
                                               [](auto lhs, auto rhs) {
                                                   return lhs.get() < rhs.get();
                                               });
                 });

    return ret_vec;
}


auto SelectionBucketCreator::countNodesAbleToUseBucketWithout(SelectionBucket bucket,
                                                              const std::vector<NodeSelection>& selections) const noexcept
    -> std::size_t
{
    for(const auto& s : selections) {
        bucket = std::move(bucket).exclude(s);
    }

    return countNodesAbleToUseBucket(bucket);
}
