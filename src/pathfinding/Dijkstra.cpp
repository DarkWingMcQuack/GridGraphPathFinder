#include <functional>
#include <graph/GridGraph.hpp>
#include <numeric>
#include <optional>
#include <pathfinding/Distance.hpp>
#include <pathfinding/Dijkstra.hpp>
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
      distances_(graph.width * graph.height,
                 UNREACHABLE),
      settled_(graph.width * graph.height, false),
      pq_(DijkstraQueueComparer{}) {}


auto Dijkstra::findAllRoutes(const graph::Node& source, const graph::Node& target) noexcept
    -> std::vector<Path>
{
    [[maybe_unused]] auto _ = computeDistance(source, target);
    return extractAllShortestPaths(source, target);
}

auto Dijkstra::findRoute(const graph::Node& source, const graph::Node& target) noexcept
    -> std::optional<Path>
{
    [[maybe_unused]] auto _ = computeDistance(source, target);
    return extractShortestPath(source, target);
}

auto Dijkstra::findDistance(const graph::Node& source, const graph::Node& target) noexcept
    -> Distance
{
    return computeDistance(source, target);
}

auto Dijkstra::findTrivialDistance(const graph::Node& source, const graph::Node& target) noexcept
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

auto Dijkstra::getIndex(const graph::Node& n) const noexcept
    -> std::optional<std::size_t>
{
    auto row = n.row;
    auto column = n.column;

    if(row >= graph_.get().height || row < 0) {
        return std::nullopt;
    }

    if(column >= graph_.get().width || column < 0) {
        return std::nullopt;
    }

    return n.row * graph_.get().width + n.column;
}

auto Dijkstra::getDistanceTo(const graph::Node& n) const noexcept
    -> Distance
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        return distances_[index_opt.value()];
    }

    return UNREACHABLE;
}


auto Dijkstra::setDistanceTo(const graph::Node& n, Distance distance) noexcept
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        distances_[index_opt.value()] = distance;
    }
}

auto Dijkstra::extractAllShortestPaths(const graph::Node& source, const graph::Node& target) const noexcept
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

auto Dijkstra::extractShortestPath(const graph::Node& source, const graph::Node& target) const noexcept
    -> std::optional<Path>
{
    //check if a path exists
    if(UNREACHABLE == getDistanceTo(target)) {
        return std::nullopt;
    }

    Path path{std::vector{target}};

    while(path.getSource() != source) {
        const auto& last_inserted = path.getSource();
        auto neigbours = graph_.get().getWalkableNeigbours(last_inserted);

        auto min_iter = std::min_element(
            std::begin(neigbours),
            std::end(neigbours),
            [&](const auto& lhs, const auto& rhs) {
                return getDistanceTo(lhs) < getDistanceTo(rhs);
            });

        //this can never happen
        if(std::end(neigbours) == min_iter) {
            return std::nullopt;
        }

        auto min_neig = *min_iter;

        //this can also never happen
        if(getDistanceTo(min_neig) == UNREACHABLE) {
            return std::nullopt;
        }

        path.pushFront(min_neig);
    }

    return path;
}


auto Dijkstra::reset() noexcept
    -> void
{
    for(auto n : touched_) {
        unSettle(n);
        setDistanceTo(n, UNREACHABLE);
    }
    touched_.clear();
    pq_ = DijkstraQueue{DijkstraQueueComparer{}};
}

auto Dijkstra::unSettle(const graph::Node& n)
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        settled_[index_opt.value()] = false;
    }
}

auto Dijkstra::settle(const graph::Node& n) noexcept
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        settled_[index_opt.value()] = true;
    }
}

auto Dijkstra::isSettled(const graph::Node& n)
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

auto Dijkstra::computeDistance(const graph::Node& source, const graph::Node& target) noexcept
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
            }
        }
    }

    return getDistanceTo(target);
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
