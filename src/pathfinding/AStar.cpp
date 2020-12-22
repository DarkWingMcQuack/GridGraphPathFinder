#include <functional>
#include <graph/GridGraph.hpp>
#include <numeric>
#include <optional>
#include <pathfinding/AStar.hpp>
#include <pathfinding/Distance.hpp>
#include <queue>
#include <string_view>
#include <vector>

using graph::Node;
using graph::GridGraph;
using pathfinding::AStar;
using pathfinding::Path;
using graph::Distance;
using graph::UNREACHABLE;

AStar::AStar(const graph::GridGraph& graph) noexcept
    : graph_(graph),
      distances_(graph.size(), UNREACHABLE),
      settled_(graph.size(), false),
      pq_(AStarQueueComparer{}),
      before_(graph.size(), graph::NOT_REACHABLE) {}


auto AStar::findRoute(graph::Node source, graph::Node target) noexcept
    -> std::optional<Path>
{
    [[maybe_unused]] auto _ = computeDistance(source, target);
    return extractShortestPath(source, target);
}

auto AStar::findDistance(graph::Node source, graph::Node target) noexcept
    -> Distance
{
    return computeDistance(source, target);
}

auto AStar::findTrivialDistance(graph::Node source, graph::Node target) noexcept
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

auto AStar::getDistanceTo(graph::Node n) const noexcept
    -> Distance
{
    auto index = graph_.get().nodeToIndex(n);
    return distances_[index];
}


auto AStar::setDistanceTo(graph::Node n, Distance distance) noexcept
    -> void
{
    auto index = graph_.get().nodeToIndex(n);
    distances_[index] = distance;
}

auto AStar::extractShortestPath(graph::Node source, graph::Node target) const noexcept
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


auto AStar::reset() noexcept
    -> void
{
    for(auto n : touched_) {
        unSettle(n);
        setDistanceTo(n, UNREACHABLE);
        setBefore(n, graph::NOT_REACHABLE);
    }

    touched_.clear();
    pq_ = AStarQueue{AStarQueueComparer{}};
}

auto AStar::unSettle(graph::Node n)
    -> void
{
    auto index = graph_.get().nodeToIndex(n);
    settled_[index] = false;
}

auto AStar::settle(graph::Node n) noexcept
    -> void
{
    auto index = graph_.get().nodeToIndex(n);
    settled_[index] = true;
}

auto AStar::isSettled(graph::Node n)
    -> bool
{
    auto index = graph_.get().nodeToIndex(n);
    return settled_[index];
}

auto AStar::computeDistance(graph::Node source, graph::Node target) noexcept
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
        auto trivial_distance = findTrivialDistance(source, target);
        pq_.emplace(source, 0l, trivial_distance);
        setDistanceTo(source, 0);
        touched_.emplace_back(source);
    }

    while(!pq_.empty()) {
        auto [current_node, current_dist, _] = pq_.top();

        settle(current_node);

        if(current_node == target) {
            return current_dist;
        }

        //pop after the return, otherwise we loose a value
        //when reusing the pq
        pq_.pop();

        auto neigbours = graph_.get().getManhattanNeigbours(current_node);

        for(auto neig : neigbours) {
            if(graph_.get().isBarrier(neig)) {
                continue;
            }

            auto neig_dist = getDistanceTo(neig);
            auto neig_heuristic = findTrivialDistance(neig, target);
            auto new_dist = current_dist + 1;

            if(UNREACHABLE != current_dist and neig_dist > new_dist) {
                touched_.emplace_back(neig);
                setDistanceTo(neig, new_dist);
                pq_.emplace(neig, new_dist, neig_heuristic);
                setBefore(neig, current_node);
            }
        }
    }

    return getDistanceTo(target);
}


auto AStar::setBefore(graph::Node n, graph::Node before) noexcept
    -> void
{
    auto idx = graph_.get().nodeToIndex(n);
    before_[idx] = before;
}
