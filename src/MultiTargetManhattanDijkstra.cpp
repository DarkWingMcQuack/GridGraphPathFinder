#include <GridGraph.hpp>
#include <MultiTargetManhattanDijkstra.hpp>
#include <Path.hpp>
#include <SimpleDijkstra.hpp>
#include <functional>
#include <optional>
#include <queue>
#include <string_view>
#include <vector>

using graph::Node;
using graph::GridGraph;
using pathfinding::MultiTargetManhattanDijkstra;
using pathfinding::Distance;

MultiTargetManhattanDijkstra::MultiTargetManhattanDijkstra(const graph::GridGraph& graph)
    : ManhattanDijkstra::ManhattanDijkstra(graph),
      settled_(graph.width * graph.height, false) {}

auto MultiTargetManhattanDijkstra::settle(const graph::Node& n)
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        settled_[index_opt.value()] = true;
    }
}

auto MultiTargetManhattanDijkstra::unSettle(const graph::Node& n)
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        settled_[index_opt.value()] = false;
    }
}

auto MultiTargetManhattanDijkstra::isSettled(const graph::Node& n)
    -> std::optional<bool>
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        return settled_[index_opt.value()];
    }

    return std::nullopt;
}

auto MultiTargetManhattanDijkstra::resetSettlements(const std::vector<graph::Node>& touched)
    -> void
{
    for(const auto& n : touched) {
        unSettle(n);
    }
}

auto MultiTargetManhattanDijkstra::computeDistances(const graph::Node& source,
                                                    const grid::GridCell& targets) noexcept
    -> std::vector<graph::Node>
{
    using graph::Node;

    DijkstraQueue queue(DIJKSTRA_QUEUE_COMPERATOR);
    queue.emplace(source, 0l);
    setDistanceTo(source, 0l);

    std::vector<Node> touched;
    std::size_t settled_targets{0};

    while(!queue.empty()) {
        auto [current_node, current_dist] = queue.top();
        queue.pop();

        auto is_already_settled_opt = isSettled(current_node);
        if(targets.isInCell(current_node) //if the current node is a target
           //which was not settled before
           && is_already_settled_opt
           && !is_already_settled_opt.value()) {
            settled_targets++;
        }

        if(settled_targets >= targets.size()) {
            return touched;
        }

        settle(current_node);

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

auto MultiTargetManhattanDijkstra::findDistances(const graph::Node& source,
                                                 const grid::GridCell& targets) noexcept
    -> std::vector<Distance>
{
    auto touched = computeDistances(source, targets);


    std::vector<Distance> distances;
    std::transform(std::cbegin(targets),
                   std::cend(targets),
                   std::back_inserter(distances),
                   [&](const auto& target) {
                       return getDistanceTo(target);
                   });

    resetDistances(touched);
    resetSettlements(touched);

    return distances;
}

auto MultiTargetManhattanDijkstra::findRoutes(const graph::Node& source,
                                              const grid::GridCell& targets) noexcept
    -> std::vector<std::vector<Path>>
{
    auto touched = computeDistances(source, targets);

    std::vector<std::vector<Path>> paths;
    std::transform(std::cbegin(targets),
                   std::cend(targets),
                   std::back_inserter(paths),
                   [&](const auto& target) {
                       return extractShortestPaths(source, target);
                   });

    resetDistances(touched);
    resetSettlements(touched);

    return paths;
}
