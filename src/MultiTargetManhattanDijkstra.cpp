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
