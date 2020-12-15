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
      bucket_lookup_(graph_.size(), std::vector<std::size_t>{}),
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

    std::transform(std::begin(graph_),
                   std::end(graph_),
                   std::back_inserter(incomplete_nodes_),
                   [&](auto node) {
                       return graph_.nodeToIndex(node);
                   });
}

SelectionBucketCreator::SelectionBucketCreator(SelectionLookup&& lookup)
    : SelectionBucketCreator::SelectionBucketCreator{lookup.graph_,
                                                     std::move(lookup.selections_),
                                                     std::move(lookup.selection_lookup_)}
{
    std::transform(std::begin(graph_),
                   std::end(graph_),
                   std::back_inserter(incomplete_nodes_),
                   [&](auto node) {
                       return graph_.nodeToIndex(node);
                   });
}

auto SelectionBucketCreator::getIncompleteNodeIdx() const noexcept
    -> std::optional<std::size_t>
{
    if(incomplete_nodes_.empty()) {
        return std::nullopt;
    }

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::size_t> dist(0u, incomplete_nodes_.size() - 1);

    return incomplete_nodes_[dist(rng)];
}


auto SelectionBucketCreator::createBucketLookup() && noexcept
    -> SelectionBucketLookup
{

    fmt::print("number of selections: {}\n", selections_.size());

    for(auto node : graph_) {
        auto idx = graph_.nodeToIndex(node);
        fmt::print("size: {}\n", selections_per_node_[idx].size());
    }

    fmt::print("-----------------------------------------------\n");

    while(auto current_idx_opt = getIncompleteNodeIdx()) {
        const auto current_idx = current_idx_opt.value();
        auto current_bucket = buildBuckedContainingAllOfNode(current_idx);
        current_bucket = optimizeBucket(std::move(current_bucket));

        addBucket(std::move(current_bucket));
    }

    fmt::print("number of buckets: {}\n", buckets_.size());

    pruneSelectionBuckets();

    for(auto node : graph_) {
        auto idx = graph_.nodeToIndex(node);
        fmt::print("size: {}\n", bucket_lookup_[idx].size());
    }

    fmt::print("BUCKET SIZED:\n");

    for(const auto& bucket : buckets_) {
        fmt::print("bucket size: {}\n", bucket.getSelections().size());
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
            if(number_of_conficts == 0) {
                continue;
            }

            auto nodes_using_new_bucket = countNodesAbleToUseBucketWithout(bucket, conflicts);
            auto nodes_using_old_bucket = countNodesAbleToUseBucket(bucket);

            auto old_bucket_size = bucket.getSelections().size();

            auto bucket_size_ratio = static_cast<double>(number_of_conficts)
                / static_cast<double>(old_bucket_size);

            auto useage_ratio = static_cast<double>(nodes_using_new_bucket - nodes_using_old_bucket)
                / static_cast<double>(nodes_using_new_bucket);

            auto new_ratio = useage_ratio / bucket_size_ratio;

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

    fmt::print("found bucket with size: {} which is used by {} nodes\n",
               bucket.getSelections().size(),
               countNodesAbleToUseBucket(bucket));

    fmt::print("------------------------------------------------\n");
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
    const auto& bucket_ref = buckets_.back();
    auto bucket_idx = buckets_.size() - 1;

    // TODO: do in parallel
    // for(std::size_t i{0}; i < selections_per_node_.size(); i++) {
    for(auto node : graph_) {
        auto i = graph_.nodeToIndex(node);
        auto& selections = selections_per_node_[i];

        if(!selections.empty() and bucket_ref.isSubSetOf(selections)) {
            selections.erase(
                std::remove_if(std::begin(selections),
                               std::end(selections),
                               [&](const auto& selection) {
                                   return bucket_ref.contains(selection);
                               }),
                std::end(selections));

            bucket_lookup_[i].emplace_back(bucket_idx);
        }
    }

    incomplete_nodes_.erase(
        std::remove_if(std::begin(incomplete_nodes_),
                       std::end(incomplete_nodes_),
                       [&](auto id) {
                           return selections_per_node_[id].empty();
                       }),
        std::end(incomplete_nodes_));
}

auto SelectionBucketCreator::countNodesAbleToUseBucketWith(const SelectionBucket& bucket,
                                                           const NodeSelection& selection) const noexcept
    -> std::size_t
{
    std::vector<graph::Node> nodes;
    std::copy_if(std::begin(graph_),
                 std::end(graph_),
                 std::back_inserter(nodes),
                 [&](auto node) {
                     auto idx = graph_.nodeToIndex(node);
                     const auto& selections = selections_per_node_[idx];
                     return !selections.empty() and bucket.isSubSetOf(selections);
                 });

    nodes.erase(
        std::remove_if(
            std::begin(nodes),
            std::end(nodes),
            [&](auto node) {
                auto idx = graph_.nodeToIndex(node);
                const auto& selections = selections_per_node_[idx];
                return std::binary_search(
                    std::begin(selections),
                    std::end(selections),
                    std::ref(selection),
                    [&](const auto& lhs, const auto& rhs) {
                        return lhs.get() < rhs.get();
                    });
            }),
        std::end(nodes));

    return nodes.size();
}

auto SelectionBucketCreator::countNodesAbleToUseBucket(const SelectionBucket& bucket) const noexcept
    -> std::size_t
{
    return std::count_if(std::begin(graph_),
                         std::end(graph_),
                         [&](auto node) {
                             auto idx = graph_.nodeToIndex(node);
                             const auto& selections = selections_per_node_[idx];
                             return !selections.empty() and bucket.isSubSetOf(selections);
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

auto SelectionBucketCreator::pruneSelectionBuckets() noexcept
    -> void
{
    for(auto& bucket_ids : bucket_lookup_) {
        auto before = bucket_ids.size();
        bucket_ids.erase(
            std::remove_if(std::begin(bucket_ids),
                           std::end(bucket_ids),
                           [&](auto outer_id) {
                               return std::any_of(std::cbegin(bucket_ids),
                                                  std::cend(bucket_ids),
                                                  [&](auto inner_id) {
                                                      const auto& outer = buckets_[outer_id];
                                                      const auto& inner = buckets_[inner_id];
                                                      if(outer_id == inner_id) {
                                                          return false;
                                                      }
                                                      return outer.isSubSetOf(inner);
                                                  });
                           }),
            std::end(bucket_ids));

        fmt::print("pruned {} buckets\n", before - bucket_ids.size());
    }
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
