#include <functional>
#include <graph/GridGraph.hpp>
#include <numeric>
#include <optional>
#include <pathfinding/Dijkstra.hpp>
#include <pathfinding/Distance.hpp>
#include <queue>
#include <string_view>
#include <vector>

using graph::Node;
using graph::GridGraph;
using pathfinding::GridGraphDijkstra;
using pathfinding::Path;
using graph::Distance;
using graph::UNREACHABLE;

GridGraphDijkstra::GridGraphDijkstra(const graph::GridGraph& graph) noexcept
    : graph_(graph),
      distances_(graph.size(), UNREACHABLE),
      settled_(graph.size(), false),
      pq_(DijkstraQueueComparer{}),
      before_(graph.size(), graph::NOT_REACHABLE) {}

auto GridGraphDijkstra::findRoute(graph::Node source, graph::Node target) noexcept
    -> std::optional<Path>
{
    [[maybe_unused]] auto _ = computeDistance(source, target);
    return extractShortestPath(source, target);
}

auto GridGraphDijkstra::findDistance(graph::Node source, graph::Node target) noexcept
    -> Distance
{
    return computeDistance(source, target);
}

auto GridGraphDijkstra::findTrivialDistance(graph::Node source, graph::Node target) noexcept
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

auto GridGraphDijkstra::getDistanceTo(graph::Node n) const noexcept
    -> Distance
{
    auto index = graph_.get().nodeToIndex(n);
    return distances_[index];
}


auto GridGraphDijkstra::setDistanceTo(graph::Node n, Distance distance) noexcept
    -> void
{
    auto index = graph_.get().nodeToIndex(n);
    distances_[index] = distance;
}

auto GridGraphDijkstra::extractShortestPath(graph::Node source, graph::Node target) const noexcept
    -> std::optional<Path>
{
    //check if a path exists
    if(UNREACHABLE == getDistanceTo(target)) {
        return std::nullopt;
    }

    Path path{std::vector{target}};

    while(path.getSource() != source) {
        const auto& last_inserted = path.getSource();
        auto last_inserted_idx = graph_.get().nodeToIndex(last_inserted);

        path.pushFront(before_[last_inserted_idx]);
    }

    return path;
}


auto GridGraphDijkstra::reset() noexcept
    -> void
{
    for(auto n : touched_) {
        unSettle(n);
        setDistanceTo(n, UNREACHABLE);
        setBefore(n, graph::NOT_REACHABLE);
    }

    touched_.clear();
    pq_ = DijkstraQueue{DijkstraQueueComparer{}};
}

auto GridGraphDijkstra::unSettle(graph::Node n)
    -> void
{
    auto index = graph_.get().nodeToIndex(n);
    settled_[index] = false;
}

auto GridGraphDijkstra::settle(graph::Node n) noexcept
    -> void
{
    auto index = graph_.get().nodeToIndex(n);
    settled_[index] = true;
}

auto GridGraphDijkstra::isSettled(graph::Node n)
    -> bool
{
    auto index = graph_.get().nodeToIndex(n);
    return settled_[index];
}

auto GridGraphDijkstra::computeDistance(graph::Node source, graph::Node target) noexcept
    -> Distance
{
    using graph::UNREACHABLE;

    if(graph_.get().isBarrier(source)
       or graph_.get().isBarrier(target)) {
        return UNREACHABLE;
    }

    if(source == last_source_
       && isSettled(target)) {
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

        //pop after the return, otherwise we loose a value
        //when reusing the pq
        pq_.pop();

        auto neigbours = graph_.get().getNeigboursOf(current_node);

        for(auto [neig, distance] : neigbours) {

            auto neig_dist = getDistanceTo(neig);
            auto new_dist = current_dist + distance;

            if(UNREACHABLE != current_dist and neig_dist > new_dist) {
                touched_.emplace_back(neig);
                setDistanceTo(neig, new_dist);
                pq_.emplace(neig, new_dist);
                setBefore(neig, current_node);
            }
        }
    }

    return getDistanceTo(target);
}

auto GridGraphDijkstra::setBefore(graph::Node n, graph::Node before) noexcept
    -> void
{
    auto idx = graph_.get().nodeToIndex(n);
    before_[idx] = before;
}
