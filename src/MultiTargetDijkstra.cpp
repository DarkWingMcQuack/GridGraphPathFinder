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
using pathfinding::MultiTargetDijkstra;
using pathfinding::Distance;

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
