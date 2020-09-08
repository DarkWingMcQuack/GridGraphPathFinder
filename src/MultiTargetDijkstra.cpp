#include <GridGraph.hpp>
#include <MultiTargetDijkstra.hpp>
#include <Path.hpp>
#include <functional>
#include <optional>
#include <queue>
#include <string_view>
#include <vector>

using graph::Node;
using graph::GridGraph;
using graph::Distance;
using pathfinding::MultiTargetDijkstra;

MultiTargetDijkstra::MultiTargetDijkstra(const graph::GridGraph& graph)
    : SimpleDijkstra::SimpleDijkstra(graph),
      settled_(graph.width * graph.height, false) {}

auto MultiTargetDijkstra::settle(const graph::Node& n) noexcept
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        settled_[index_opt.value()] = true;
    }
}

auto MultiTargetDijkstra::unSettle(const graph::Node& n) noexcept
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        settled_[index_opt.value()] = false;
    }
}


auto MultiTargetDijkstra::isSettled(const graph::Node& n) noexcept
    -> std::optional<bool>
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        return settled_[index_opt.value()];
    }

    return std::nullopt;
}



auto MultiTargetDijkstra::resetSettlements(const std::vector<graph::Node>& touched) noexcept
    -> void
{
    for(const auto& n : touched) {
        unSettle(n);
    }
}

auto MultiTargetDijkstra::findRoutes(const graph::Node& source,
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

auto MultiTargetDijkstra::findDistances(const graph::Node& source,
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

auto MultiTargetDijkstra::computeDistances(const graph::Node& source,
                                           const grid::GridCell& targets) noexcept
    -> std::vector<graph::Node>
{
    using graph::Node;
    using graph::UNREACHABLE;

    DijkstraQueue queue(DIJKSTRA_QUEUE_COMPERATOR);
    queue.emplace(source, 0l);
    setDistanceTo(source, 0l);

    std::vector<Node> touched{source};
    std::size_t settled_targets{0};

    while(!queue.empty()) {
        auto [current_node, current_dist] = queue.top();
        queue.pop();

        auto is_already_settled_opt = isSettled(current_node);
        if( //if the current node is a target
            targets.isInCell(current_node)
            //which was not settled before
            && is_already_settled_opt
            && !is_already_settled_opt.value()) {
            settled_targets++;
        }

        if(settled_targets >= targets.size()) {
            return touched;
        }

        settle(current_node);

        auto neigbours = getWalkableNeigboursOf(current_node);

        for(auto&& neig : neigbours) {
            touched.emplace_back(neig);

            auto neig_dist = getDistanceTo(neig);

            if(UNREACHABLE != current_dist and neig_dist > current_dist + 1) {
                setDistanceTo(neig, current_dist + 1);
                queue.emplace(neig, current_dist + 1);
            }
        }
    }

    return touched;
}
