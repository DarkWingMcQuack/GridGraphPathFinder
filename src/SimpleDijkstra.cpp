#include <GridGraph.hpp>
#include <SimpleDijkstra.hpp>
#include <functional>
#include <optional>
#include <queue>
#include <string_view>
#include <vector>

using graph::Node;
using graph::GridGraph;
using pathfinding::SimpleDijkstra;
using pathfinding::Distance;
using pathfinding::UNREACHABLE;

SimpleDijkstra::SimpleDijkstra(const graph::GridGraph& graph) noexcept
    : graph_(graph),
      distances_(graph.width * graph.height,
                 UNREACHABLE) {}


auto SimpleDijkstra::findRoutes(const graph::Node& source, const graph::Node& target) noexcept
    -> std::vector<Path>
{
    auto touched = computeDistances(source, target);
    auto path = extractShortestPaths(source, target);
    resetDistances(touched);

    return path;
}

auto SimpleDijkstra::getIndex(const graph::Node& n) const noexcept
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

auto SimpleDijkstra::getDistanceTo(const graph::Node& n) const noexcept
    -> Distance
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        return distances_[index_opt.value()];
    }

    return UNREACHABLE;
}


auto SimpleDijkstra::setDistanceTo(const graph::Node& n, Distance distance) noexcept
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        distances_[index_opt.value()] = distance;
    }
}

auto SimpleDijkstra::resetDistances(const std::vector<graph::Node>& touched) noexcept
    -> void
{
    for(const auto& node : touched) {
        setDistanceTo(node,
                      UNREACHABLE);
    }
}

auto SimpleDijkstra::extractShortestPaths(const graph::Node& source, const graph::Node& target) const noexcept
    -> std::vector<Path>
{
    //check if a path exists
    if(UNREACHABLE == getDistanceTo(target)) {
        return {};
    }

    Path initial{std::vector{target}};
    std::vector<Path> unfinished_paths{std::move(initial)};
    std::vector<Path> complete_paths;

    while(!unfinished_paths.empty()) {
        auto path = std::move(unfinished_paths.back());
        unfinished_paths.pop_back();

        const auto& last_inserted = path.getSource();
        auto neigbours = getWalkableNeigboursOf(last_inserted);
        auto last_dist = getDistanceTo(last_inserted);
        auto smallest_distance = findSmallestDistanceGreaterThan(neigbours);

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
                unfinished_paths.emplace_back(std::move(path_copy));
            }
        }
    }

    return complete_paths;
}


auto SimpleDijkstra::getWalkableNeigboursOf(const graph::Node& n) const noexcept
    -> std::vector<graph::Node>
{
    return graph_.get().getWalkableNeigbours(n);
}

auto SimpleDijkstra::getWalkableManhattanNeigboursOf(const graph::Node& n) const noexcept
    -> std::vector<graph::Node>
{
    return graph_.get().getWalkableManhattanNeigbours(n);
}

auto SimpleDijkstra::computeDistances(const graph::Node& source, const graph::Node& target) noexcept
    -> std::vector<graph::Node>
{
    DijkstraQueue queue(DIJKSTRA_QUEUE_COMPERATOR);
    queue.emplace(source, 0l);
    setDistanceTo(source, 0);

    std::vector<Node> touched;

    while(!queue.empty()) {
        auto [current_node, current_dist] = std::move(queue.top());
        queue.pop();

        if(current_node == target) {
            return touched;
        }

        auto neigbours = graph_.get().getWalkableNeigbours(current_node);

        for(auto&& neig : neigbours) {
            touched.emplace_back(neig);

            auto neig_dist = getDistanceTo(neig);

            if(neig_dist > current_dist + 1) {
                setDistanceTo(neig, current_dist + 1);
                queue.emplace(neig, current_dist + 1);
            }
        }
    }


    return touched;
}


auto SimpleDijkstra::findSmallestDistanceGreaterThan(const std::vector<graph::Node>& nodes) const noexcept
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

    std::sort(std::begin(distances),
              std::end(distances));

    return distances[0];
    // for(int i{0}; i < distances.size(); i++) {
    //     auto dist = distances[i];
    //     if(dist > limit) {
    //         return dist;
    //     }
    // }

    // return UNREACHABLE;
}
