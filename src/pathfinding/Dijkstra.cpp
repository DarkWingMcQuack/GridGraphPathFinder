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
using pathfinding::Dijkstra;
using pathfinding::Path;
using graph::Distance;
using graph::UNREACHABLE;

Dijkstra::Dijkstra(const graph::GridGraph& graph) noexcept
    : graph_(graph),
      distances_(graph.size(), UNREACHABLE),
      settled_(graph.size(), false),
      pq_(DijkstraQueueComparer{}),
      before_(graph.size(), graph::NOT_REACHABLE) {}


auto Dijkstra::findAllRoutes(graph::Node source, graph::Node target) noexcept
    -> std::vector<Path>
{
    [[maybe_unused]] auto _ = computeDistance(source, target);
    return extractAllShortestPaths(source, target);
}

auto Dijkstra::findRoute(graph::Node source, graph::Node target) noexcept
    -> std::optional<Path>
{
    [[maybe_unused]] auto _ = computeDistance(source, target);
    return extractShortestPath(source, target);
}

auto Dijkstra::findDistance(graph::Node source, graph::Node target) noexcept
    -> Distance
{
    return computeDistance(source, target);
}

auto Dijkstra::findTrivialDistance(graph::Node source, graph::Node target) noexcept
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

auto Dijkstra::getIndex(graph::Node n) const noexcept
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

auto Dijkstra::getDistanceTo(graph::Node n) const noexcept
    -> Distance
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        return distances_[index_opt.value()];
    }

    return UNREACHABLE;
}


auto Dijkstra::setDistanceTo(graph::Node n, Distance distance) noexcept
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        distances_[index_opt.value()] = distance;
    }
}

auto Dijkstra::extractAllShortestPaths(graph::Node source, graph::Node target) const noexcept
    -> std::vector<Path>
{
    //check if a path exists
    if(UNREACHABLE == getDistanceTo(target)) {
        return {};
    }

    Path initial{std::vector{target}};

    std::priority_queue unfinished(
        [](const auto& lhs, const auto& rhs) {
            return lhs.getLength() > rhs.getLength();
        },
        std::vector<Path>{std::move(initial)});

    std::vector<Path> complete_paths;

    while(!unfinished.empty()) {
        auto path = unfinished.top();
        unfinished.pop();

        if(!complete_paths.empty()
           && path.getLength() + 1 > complete_paths[0].getLength()) {
            return complete_paths;
        }

        const auto& last_inserted = path.getSource();
        auto neigbours = graph_.get().getWalkableNeigbours(last_inserted);
        auto smallest_distance = findSmallestDistance(neigbours);

        if(smallest_distance == UNREACHABLE) {
            continue;
        }

        for(const auto& neig : neigbours) {
            auto neig_dist = getDistanceTo(neig);

            if(neig_dist != smallest_distance) {
                continue;
            }


            auto path_copy = path;
            path_copy.pushFront(neig);

            if(neig == source) {
                complete_paths.emplace_back(std::move(path_copy));
            } else {
                unfinished.emplace(std::move(path_copy));
            }
        }
    }

    return complete_paths;
}

auto Dijkstra::extractShortestPath(graph::Node source, graph::Node target) const noexcept
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


auto Dijkstra::reset() noexcept
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

auto Dijkstra::unSettle(graph::Node n)
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        settled_[index_opt.value()] = false;
    }
}

auto Dijkstra::settle(graph::Node n) noexcept
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        settled_[index_opt.value()] = true;
    }
}

auto Dijkstra::isSettled(graph::Node n)
    -> bool
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        return settled_[index_opt.value()];
    }

    return false;
}

auto Dijkstra::getNodesWithMinDistanceIn(const graph::GridCell& cell) noexcept
    -> std::vector<graph::Node>
{
    auto min_dist = std::accumulate(std::begin(cell),
                                    std::end(cell),
                                    UNREACHABLE,
                                    [&](auto acc, auto node) {
                                        auto dist = getDistanceTo(node);
                                        return std::min(dist, acc);
                                    });

    std::vector<Node> nodes;
    std::copy_if(std::begin(cell),
                 std::end(cell),
                 std::back_inserter(nodes),
                 [&](const auto& node) {
                     return getDistanceTo(node) == min_dist;
                 });

    return nodes;
}


auto Dijkstra::getMinDistanceIn(const graph::GridCell& cell) noexcept
    -> graph::Distance
{
    return std::accumulate(std::begin(cell),
                           std::end(cell),
                           UNREACHABLE,
                           [&](auto acc, const auto& node) {
                               return std::min(acc, getDistanceTo(node));
                           });
}

auto Dijkstra::computeDistance(graph::Node source, graph::Node target) noexcept
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

        auto neigbours = graph_.get().getWalkableNeigbours(current_node);

        for(auto&& neig : neigbours) {
            auto neig_dist = getDistanceTo(neig);
            auto new_dist = current_dist + 1;

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


auto Dijkstra::setBefore(graph::Node n, graph::Node before) noexcept
    -> void
{
    auto idx = graph_.get().nodeToIndex(n);
    before_[idx] = before;
}


auto Dijkstra::getGraph() const noexcept
    -> const GridGraph&
{
    return graph_.get();
}

auto Dijkstra::findSmallestDistance(const std::vector<graph::Node>& nodes) const noexcept
    -> Distance
{
    if(nodes.empty()) {
        return UNREACHABLE;
    }

    std::vector<Distance> distances;
    std::transform(std::cbegin(nodes),
                   std::cend(nodes),
                   std::back_inserter(distances),
                   [&](const auto& n) {
                       return getDistanceTo(n);
                   });

    return *std::min_element(std::cbegin(distances),
                             std::cend(distances));
}
