#include <GridGraph.hpp>
#include <MultiTargetDijkstra.hpp>
#include <functional>
#include <optional>
#include <queue>
#include <string_view>
#include <vector>

using graph::Node;
using graph::GridGraph;
using pathfinding::MultiTargetDijkstra;

MultiTargetDijkstra::MultiTargetDijkstra(const graph::GridGraph& graph)
    : SimpleDijkstra::SimpleDijkstra(graph) {}


auto MultiTargetDijkstra::findRoutes(const graph::Node& source, const std::vector<graph::Node>& targets)
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


auto MultiTargetDijkstra::settle(const graph::Node& n)
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        settled_[index_opt.value()] = true;
    }
}

auto MultiTargetDijkstra::unSettle(const graph::Node& n)
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        settled_[index_opt.value()] = false;
    }
}


auto MultiTargetDijkstra::isSettled(const graph::Node& n)
    -> std::optional<bool>
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        return settled_[index_opt.value()];
    }

    return std::nullopt;
}

namespace {
constexpr auto dijkstra_comperator = [](const std::pair<Node, std::int64_t>& lhs,
                                        const std::pair<Node, std::int64_t>& rhs) {
    return lhs.second < rhs.second;
};

using DijkstraQueue = std::priority_queue<std::pair<Node, std::int64_t>,
                                          std::vector<std::pair<Node, std::int64_t>>,
                                          decltype(dijkstra_comperator)>;
} // namespace

auto MultiTargetDijkstra::computeDistances(const graph::Node& source, const std::vector<graph::Node>& targets)
    -> std::vector<graph::Node>
{
    DijkstraQueue queue(dijkstra_comperator);
    queue.emplace(source, 0l);

    std::vector<Node> touched;
    std::size_t settled_targets{0};

    while(!queue.empty()) {
        auto [current_node, current_dist] = std::move(queue.top());
        queue.pop();

        auto is_already_settled_opt = isSettled(current_node);
        if( //if the current node is a target
            std::find(std::cbegin(targets),
                      std::cend(targets),
                      current_node)
                != std::cend(targets)
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

            auto neig_dist = getDistanceTo(neig).value();

            if(neig_dist == -1 || neig_dist > current_dist + 1) {
                setDistanceTo(neig, current_dist + 1);
                queue.emplace(neig, current_dist + 1);
            }
        }
    }

    return touched;
}

auto MultiTargetDijkstra::resetSettlements(const std::vector<graph::Node>& touched)
    -> void
{
    for(const auto& n : touched) {
        unSettle(n);
    }
}
