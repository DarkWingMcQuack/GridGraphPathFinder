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
                                               std::vector<std::vector<std::size_t>> left_selections,
                                               std::vector<std::vector<std::size_t>> right_selections)
    : graph_(graph),
      bucket_lookup_(graph_.size()),
      left_buckets_(graph_.size()),
      right_buckets_(graph_.size()),
      selections_(std::move(selections)),
      left_selections_(std::move(left_selections)),
      right_selections_(std::move(right_selections))
{
    std::transform(std::begin(graph_),
                   std::end(graph_),
                   std::back_inserter(incomplete_nodes_left_),
                   [&](auto node) {
                       return graph_.nodeToIndex(node);
                   });

    std::transform(std::begin(graph_),
                   std::end(graph_),
                   std::back_inserter(incomplete_nodes_right_),
                   [&](auto node) {
                       return graph_.nodeToIndex(node);
                   });
}

SelectionBucketCreator::SelectionBucketCreator(SelectionLookup&& lookup)
    : SelectionBucketCreator::SelectionBucketCreator{lookup.graph_,
                                                     std::move(lookup.selections_),
                                                     std::move(lookup.left_selections_),
                                                     std::move(lookup.right_selections_)}
{
}

auto SelectionBucketCreator::getIncompleteNodeIdxLeft() const noexcept
    -> std::optional<std::size_t>
{
    if(incomplete_nodes_left_.empty()) {
        return std::nullopt;
    }

    static std::random_device dev;
    static std::mt19937 rng(dev());
    std::uniform_int_distribution<std::size_t> dist(0u, incomplete_nodes_left_.size() - 1);

    return incomplete_nodes_left_[dist(rng)];
}

auto SelectionBucketCreator::getIncompleteNodeIdxRight() const noexcept
    -> std::optional<std::size_t>
{
    if(incomplete_nodes_right_.empty()) {
        return std::nullopt;
    }

    static std::random_device dev;
    static std::mt19937 rng(dev());
    std::uniform_int_distribution<std::size_t> dist(0u, incomplete_nodes_right_.size() - 1);

    return incomplete_nodes_right_[dist(rng)];
}


auto SelectionBucketCreator::createBucketLookup() && noexcept
    -> SelectionBucketLookup
{

    fmt::print("number of selections: {}\n", selections_.size());

    while(auto current_idx_opt = getIncompleteNodeIdxLeft()) {
        const auto current_idx = current_idx_opt.value();
        auto current_bucket = buildBuckedContainingAllOfNodeLeft(current_idx);
        current_bucket = optimizeBucketLeft(std::move(current_bucket));

        addBucketLeft(std::move(current_bucket));
    }

    while(auto current_idx_opt = getIncompleteNodeIdxRight()) {
        const auto current_idx = current_idx_opt.value();
        auto current_bucket = buildBuckedContainingAllOfNodeRight(current_idx);
        current_bucket = optimizeBucketRight(std::move(current_bucket));

        addBucketRight(std::move(current_bucket));
    }

    fmt::print("number of buckets: {}\n", buckets_.size());

    fmt::print("bucket sum: {}\n",
               std::accumulate(std::begin(buckets_),
                               std::end(buckets_),
                               0,
                               [](auto current, const auto& next) {
                                   return current + next.size();
                               }));

    return SelectionBucketLookup{graph_,
                                 std::move(buckets_),
                                 std::move(left_buckets_),
                                 std::move(right_buckets_)};
}

auto SelectionBucketCreator::optimizeBucketLeft(SelectionBucket bucket) const noexcept
    -> SelectionBucket
{
    auto changed = true;
    while(changed) {
        changed = false;
        auto best_ratio = 1.0;
        std::vector<std::size_t> best_erase;
        for(auto node : graph_) {
            auto idx = graph_.nodeToIndex(node);

            if(isCompleteLeft(idx)) {
                continue;
            }

            const auto& selections = left_selections_[idx];
            auto conflicts = calculateConflictingSelections(bucket, selections);

            const auto number_of_conficts = conflicts.size();
            if(number_of_conficts == 0) {
                continue;
            }

            auto nodes_using_new_bucket = countNodesAbleToUseBucketWithoutLeft(bucket, conflicts);
            auto nodes_using_old_bucket = countNodesAbleToUseBucketLeft(bucket);

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

        for(auto i : best_erase) {
            bucket = std::move(bucket).exclude(i);
        }
    }

    return bucket;
}

auto SelectionBucketCreator::optimizeBucketRight(SelectionBucket bucket) const noexcept
    -> SelectionBucket
{
    auto changed = true;
    while(changed) {
        changed = false;
        auto best_ratio = 1.0;
        std::vector<std::size_t> best_erase;
        for(auto node : graph_) {
            auto idx = graph_.nodeToIndex(node);

            if(isCompleteRight(idx)) {
                continue;
            }

            const auto& selections = right_selections_[idx];
            auto conflicts = calculateConflictingSelections(bucket, selections);

            const auto number_of_conficts = conflicts.size();
            if(number_of_conficts == 0) {
                continue;
            }

            auto nodes_using_new_bucket = countNodesAbleToUseBucketWithoutRight(bucket, conflicts);
            auto nodes_using_old_bucket = countNodesAbleToUseBucketRight(bucket);

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

        for(auto i : best_erase) {
            bucket = std::move(bucket).exclude(i);
        }
    }

    return bucket;
}

auto SelectionBucketCreator::isCompleteLeft(std::size_t idx) const noexcept
    -> bool
{
    return left_selections_[idx].empty();
}

auto SelectionBucketCreator::isCompleteRight(std::size_t idx) const noexcept
    -> bool
{
    return right_selections_[idx].empty();
}

auto SelectionBucketCreator::addBucketLeft(SelectionBucket bucket) noexcept
    -> void
{
    auto bucket_idx = buckets_.size();

    // TODO: do in parallel
    // for(std::size_t i{0}; i < selections_per_node_.size(); i++) {
    for(auto node : graph_) {
        auto i = graph_.nodeToIndex(node);
        auto& selection_indices = left_selections_[i];

        if(!selection_indices.empty() and bucketViableForLeft(bucket, i)) {
            selection_indices.erase(
                std::remove_if(std::begin(selection_indices),
                               std::end(selection_indices),
                               [&](auto selection_idx) {
                                   return bucket.contains(selection_idx);
                               }),
                std::end(selection_indices));

            left_buckets_[i].emplace_back(bucket_idx);
        }
    }

    buckets_.emplace_back(std::move(bucket));

    incomplete_nodes_left_.erase(
        std::remove_if(std::begin(incomplete_nodes_left_),
                       std::end(incomplete_nodes_left_),
                       [&](auto idx) {
                           return isCompleteLeft(idx);
                       }),
        std::end(incomplete_nodes_left_));
}

auto SelectionBucketCreator::addBucketRight(SelectionBucket bucket) noexcept
    -> void
{
    auto bucket_idx = buckets_.size();

    // TODO: do in parallel
    // for(std::size_t i{0}; i < selections_per_node_.size(); i++) {
    for(auto node : graph_) {
        auto i = graph_.nodeToIndex(node);
        auto& selection_indices = right_selections_[i];

        if(!selection_indices.empty() and bucketViableForRight(bucket, i)) {
            selection_indices.erase(
                std::remove_if(std::begin(selection_indices),
                               std::end(selection_indices),
                               [&](auto selection_idx) {
                                   return bucket.contains(selection_idx);
                               }),
                std::end(selection_indices));

            right_buckets_[i].emplace_back(bucket_idx);
        }
    }

    buckets_.emplace_back(std::move(bucket));

    incomplete_nodes_right_.erase(
        std::remove_if(std::begin(incomplete_nodes_right_),
                       std::end(incomplete_nodes_right_),
                       [&](auto idx) {
                           return isCompleteRight(idx);
                       }),
        std::end(incomplete_nodes_right_));
}

auto SelectionBucketCreator::countNodesAbleToUseBucketLeft(const SelectionBucket& bucket) const noexcept
    -> std::size_t
{
    return std::count_if(std::begin(graph_),
                         std::end(graph_),
                         [&](auto node) {
                             auto idx = graph_.nodeToIndex(node);
                             return !left_selections_[idx].empty() and bucketViableForLeft(bucket, idx);
                         });
}

auto SelectionBucketCreator::countNodesAbleToUseBucketRight(const SelectionBucket& bucket) const noexcept
    -> std::size_t
{
    return std::count_if(std::begin(graph_),
                         std::end(graph_),
                         [&](auto node) {
                             auto idx = graph_.nodeToIndex(node);
                             return !right_selections_[idx].empty() and bucketViableForRight(bucket, idx);
                         });
}

auto SelectionBucketCreator::bucketViableForLeft(const SelectionBucket& bucket,
                                                 std::size_t node_idx) const noexcept
    -> bool
{
    const auto& selection_indices = left_selections_[node_idx];
    const auto& bucket_selections = bucket.getSelections();

    if(bucket_selections.size() > selection_indices.size()) {
        return false;
    }

    return bucket.isSubSetOf(selection_indices);
}

auto SelectionBucketCreator::bucketViableForRight(const SelectionBucket& bucket,
                                                  std::size_t node_idx) const noexcept
    -> bool
{
    const auto& selection_indices = right_selections_[node_idx];
    const auto& bucket_selections = bucket.getSelections();

    if(bucket_selections.size() > selection_indices.size()) {
        return false;
    }

    return bucket.isSubSetOf(selection_indices);
}

auto SelectionBucketCreator::countNodesUsingLeft(std::size_t selection_idx) const noexcept
    -> std::size_t
{
    return std::count_if(std::begin(left_selections_),
                         std::end(left_selections_),
                         [&](auto selection_indices) {
                             return std::binary_search(std::begin(selection_indices),
                                                       std::end(selection_indices),
                                                       selection_idx);
                         });
}

auto SelectionBucketCreator::countNodesUsingRight(std::size_t selection_idx) const noexcept
    -> std::size_t
{
    return std::count_if(std::begin(right_selections_),
                         std::end(right_selections_),
                         [&](auto selection_indices) {
                             return std::binary_search(std::begin(selection_indices),
                                                       std::end(selection_indices),
                                                       selection_idx);
                         });
}

auto SelectionBucketCreator::buildBuckedContainingAllOfNodeLeft(std::size_t idx) const noexcept
    -> SelectionBucket
{
    auto selection_indices = left_selections_[idx];

    return SelectionBucket{std::move(selection_indices)};
}

auto SelectionBucketCreator::buildBuckedContainingAllOfNodeRight(std::size_t idx) const noexcept
    -> SelectionBucket
{
    auto selection_indices = right_selections_[idx];

    return SelectionBucket{std::move(selection_indices)};
}

auto SelectionBucketCreator::calculateConflictingSelections(const SelectionBucket& bucket,
                                                            const std::vector<std::size_t>& selection_indices) const noexcept
    -> std::vector<std::size_t>
{
    const auto& bucket_selections = bucket.getSelections();

    std::vector<std::size_t> ret_vec;
    std::copy_if(std::begin(bucket_selections),
                 std::end(bucket_selections),
                 std::back_inserter(ret_vec),
                 [&](auto idx) {
                     return !std::binary_search(std::begin(selection_indices),
                                                std::end(selection_indices),
                                                idx);
                 });

    return ret_vec;
}

auto SelectionBucketCreator::countNodesAbleToUseBucketWithoutLeft(SelectionBucket bucket,
                                                                  const std::vector<std::size_t>& indices) const noexcept
    -> std::size_t
{
    for(const auto& i : indices) {
        bucket = std::move(bucket).exclude(i);
    }

    return countNodesAbleToUseBucketLeft(bucket);
}

auto SelectionBucketCreator::countNodesAbleToUseBucketWithoutRight(SelectionBucket bucket,
                                                                   const std::vector<std::size_t>& indices) const noexcept
    -> std::size_t
{
    for(const auto& i : indices) {
        bucket = std::move(bucket).exclude(i);
    }

    return countNodesAbleToUseBucketRight(bucket);
}
