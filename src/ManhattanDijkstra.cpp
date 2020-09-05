#include <GridGraph.hpp>
#include <ManhattanDijkstra.hpp>
#include <SimpleDijkstra.hpp>
#include <functional>
#include <optional>
#include <queue>
#include <string_view>
#include <vector>

using graph::Node;
using graph::GridGraph;
using pathfinding::ManhattanDijkstra;
using pathfinding::Distance;
using pathfinding::UNREACHABLE;

ManhattanDijkstra::ManhattanDijkstra(const graph::GridGraph& graph) noexcept
    : graph_(graph),
      distances_(graph.width * graph.height,
                 UNREACHABLE) {}


auto ManhattanDijkstra::findRoutes(const graph::Node& source, const graph::Node& target) noexcept
    -> std::vector<Path>
{
    auto touched = computeDistances(source, target);
    auto path = extractShortestPaths(source, target);
    resetDistances(touched);

    return path;
}

auto ManhattanDijkstra::findDistance(const graph::Node& source, const graph::Node& target) noexcept
    -> Distance
{
    auto touched = computeDistances(source, target);
    auto distance = getDistanceTo(target);
    resetDistances(touched);

    return distance;
}

auto ManhattanDijkstra::getIndex(const graph::Node& n) const noexcept
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

auto ManhattanDijkstra::getDistanceTo(const graph::Node& n) const noexcept
    -> Distance
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        return distances_[index_opt.value()];
    }

    return UNREACHABLE;
}


auto ManhattanDijkstra::setDistanceTo(const graph::Node& n, Distance distance) noexcept
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        distances_[index_opt.value()] = distance;
    }
}

auto ManhattanDijkstra::resetDistances(const std::vector<graph::Node>& touched) noexcept
    -> void
{
    for(const auto& node : touched) {
        setDistanceTo(node,
                      UNREACHABLE);
    }
}

auto ManhattanDijkstra::extractShortestPaths(const graph::Node& source, const graph::Node& target) const noexcept
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
        auto neigbours = getWalkableManhattanNeigboursOf(last_inserted);
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


auto ManhattanDijkstra::getWalkableManhattanNeigboursOf(const graph::Node& n) const noexcept
    -> std::vector<graph::Node>
{
    return graph_.get().getWalkableManhattanNeigbours(n);
}

auto ManhattanDijkstra::computeDistances(const graph::Node& source, const graph::Node& target) noexcept
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

        auto neigbours = getWalkableManhattanNeigboursOf(current_node);

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


auto ManhattanDijkstra::findSmallestDistance(const std::vector<graph::Node>& nodes) const noexcept
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
