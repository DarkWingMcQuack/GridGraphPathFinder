#include <fmt/ostream.h>
#include <functional>
#include <graph/GridGraph.hpp>
#include <numeric>
#include <optional>
#include <pathfinding/CachingDijkstra.hpp>
#include <pathfinding/Distance.hpp>
#include <progresscpp/ProgressBar.hpp>
#include <queue>
#include <string_view>
#include <vector>

using graph::Distance;
using graph::GridGraph;
using graph::Node;
using graph::UNREACHABLE;
using pathfinding::CachingGridGraphDijkstra;

CachingGridGraphDijkstra::CachingGridGraphDijkstra(const graph::GridGraph &graph) noexcept
    : graph_(graph),
      distances_(graph.size(), UNREACHABLE),
      settled_(graph.size(), false),
      pq_(DijkstraQueueComparer{}),
      cache_index_(graph.size(), 0),
      distance_cache_(graph.countWalkableNodes(),
                      std::vector(graph.countWalkableNodes(), UNREACHABLE))
{
    fmt::print("computing all to all pairs...\n");
    auto graph_size = graph.countWalkableNodes();

    std::size_t counter = 0;
    for(auto n : graph) {
        auto idx = getIndex(n).value();
        cache_index_[idx] = counter++;
    }

    progresscpp::ProgressBar bar{graph_size * graph_size, 80ul};

    for(auto from : graph) {
        for(auto to : graph) {
            auto distance = computeDistance(from, to);
            insertCache(from, to, distance);
        }
        bar += graph_size;
        bar.displayIfChangedAtLeast(0.02);
    }
    bar.done();

    //cleanup everything to save memory
    distances_.clear();
    settled_.clear();
    pq_ = DijkstraQueue{DijkstraQueueComparer{}};
    touched_.clear();
    last_source_ = std::nullopt;
}

auto CachingGridGraphDijkstra::findDistance(const graph::Node &source,
                                   const graph::Node &target) const noexcept
    -> Distance
{
    // This check is realy slow, just make sure to not query any barriers
    // if(graph_.get().isBarrier(source) or graph_.get().isBarrier(target)) {
    //     return UNREACHABLE;
    // }

    return queryCache(source, target);
}

auto CachingGridGraphDijkstra::insertCache(graph::Node first, graph::Node second,
                                  graph::Distance dist) noexcept
    -> void
{
    auto first_idx = getIndex(first).value();
    auto second_idx = getIndex(second).value();

    auto first_cache_idx = cache_index_[first_idx];
    auto second_cache_idx = cache_index_[second_idx];

    distance_cache_[first_cache_idx][second_cache_idx] = dist;
}

auto CachingGridGraphDijkstra::queryCache(graph::Node first, graph::Node second) const noexcept
    -> graph::Distance
{
    auto first_idx = getIndex(first).value();
    auto second_idx = getIndex(second).value();

    auto first_cache_idx = cache_index_[first_idx];
    auto second_cache_idx = cache_index_[second_idx];

    return distance_cache_[first_cache_idx][second_cache_idx];
}

auto CachingGridGraphDijkstra::destroy() noexcept
    -> void
{
    distances_.clear();
    settled_.clear();
    touched_.clear();
    pq_ = DijkstraQueue{DijkstraQueueComparer{}};
    cache_index_.clear();
    distance_cache_.clear();
}

auto CachingGridGraphDijkstra::findTrivialDistance(const graph::Node &source,
                                          const graph::Node &target) const noexcept
    -> graph::Distance
{
    if(graph_.get().isBarrier(source) or graph_.get().isBarrier(target)) {
        return UNREACHABLE;
    }

    auto source_row = source.row;
    auto target_row = target.row;
    auto source_column = source.column;
    auto target_column = target.column;

    return (std::max(source_row, target_row)
            - std::min(source_row, target_row))
        + (std::max(source_column, target_column)
           - std::min(source_column, target_column));
}

auto CachingGridGraphDijkstra::getIndex(const graph::Node &n) const noexcept
    -> std::optional<std::size_t>
{
    auto row = n.row;
    auto column = n.column;

    if(row >= graph_.get().getHeight() || row < 0) {
        return std::nullopt;
    }

    if(column >= graph_.get().getWidth() || column < 0) {
        return std::nullopt;
    }

    return n.row * graph_.get().getWidth() + n.column;
}

auto CachingGridGraphDijkstra::getDistanceTo(const graph::Node &n) const noexcept
    -> Distance
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        return distances_[index_opt.value()];
    }

    return UNREACHABLE;
}

auto CachingGridGraphDijkstra::setDistanceTo(const graph::Node &n,
                                    Distance distance) noexcept
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        distances_[index_opt.value()] = distance;
    }
}

auto CachingGridGraphDijkstra::reset() noexcept
    -> void
{
    for(auto n : touched_) {
        unSettle(n);
        setDistanceTo(n, UNREACHABLE);
    }
    touched_.clear();
    pq_ = DijkstraQueue{DijkstraQueueComparer{}};
}

auto CachingGridGraphDijkstra::unSettle(const graph::Node &n) noexcept
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        settled_[index_opt.value()] = false;
    }
}

auto CachingGridGraphDijkstra::settle(const graph::Node &n) noexcept
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        settled_[index_opt.value()] = true;
    }
}

auto CachingGridGraphDijkstra::isSettled(const graph::Node &n) noexcept
    -> bool
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        return settled_[index_opt.value()];
    }

    return false;
}

auto CachingGridGraphDijkstra::getWalkableNeigboursOf(graph::Node node) const noexcept
    -> std::vector<Node>
{
    return graph_.get().getWalkableNeigbours(node);
}

auto CachingGridGraphDijkstra::computeDistance(const graph::Node &source,
                                      const graph::Node &target) noexcept
    -> Distance
{
    using graph::UNREACHABLE;

    if(graph_.get().isBarrier(source) or graph_.get().isBarrier(target)) {
        return UNREACHABLE;
    }

    if(source == last_source_ && isSettled(target)) {
        return getDistanceTo(target);
    }

    if(source != last_source_) {
        last_source_ = source;
        reset();
        pq_.emplace(source, 0l);
        setDistanceTo(source, 0);
        touched_.emplace_back(source);
    }

    while(!pq_.empty()) {
        auto [current_node, current_dist] = pq_.top();

        settle(current_node);

        if(current_node == target) {
            return current_dist;
        }

        // pop after the return, otherwise we loose a value
        // when reusing the pq
        pq_.pop();

        auto neigbours = graph_.get().getWalkableNeigbours(current_node);

        for(auto &&neig : neigbours) {
            auto neig_dist = getDistanceTo(neig);
            auto new_dist = current_dist + 1;

            if(UNREACHABLE != current_dist and neig_dist > new_dist) {
                touched_.emplace_back(neig);
                setDistanceTo(neig, new_dist);
                pq_.emplace(neig, new_dist);
            }
        }
    }

    return getDistanceTo(target);
}

auto CachingGridGraphDijkstra::getGraph() const noexcept -> const GridGraph &
{
    return graph_.get();
}
