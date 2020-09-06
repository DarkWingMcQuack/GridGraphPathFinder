#pragma once

#include <Distance.hpp>
#include <GridCell.hpp>
#include <ManhattanDijkstra.hpp>
#include <Path.hpp>
#include <SimpleDijkstra.hpp>
#include <functional>
#include <optional>
#include <string_view>
#include <vector>

namespace graph {
class GridGraph;
}


namespace pathfinding {

class MultiTargetManhattanDijkstra : public ManhattanDijkstra
{
public:
    MultiTargetManhattanDijkstra(const graph::GridGraph& graph);
    MultiTargetManhattanDijkstra() = delete;
    MultiTargetManhattanDijkstra(MultiTargetManhattanDijkstra&&) = default;
    MultiTargetManhattanDijkstra(const MultiTargetManhattanDijkstra&) = delete;
    auto operator=(const MultiTargetManhattanDijkstra&) -> MultiTargetManhattanDijkstra& = delete;
    auto operator=(MultiTargetManhattanDijkstra &&) -> MultiTargetManhattanDijkstra& = delete;

    [[nodiscard]] auto findRoutes(const graph::Node& source,
                                  const grid::GridCell& targets) noexcept
        -> std::vector<std::vector<Path>>;

    [[nodiscard]] auto findDistances(const graph::Node& source,
                                     const grid::GridCell& targets) noexcept
        -> std::vector<graph::Distance>;

private:
    [[nodiscard]] auto computeDistances(const graph::Node& source,
                                        const grid::GridCell& targets) noexcept
        -> std::vector<graph::Node>;

    auto settle(const graph::Node& n)
        -> void;

    auto unSettle(const graph::Node& n)
        -> void;

    auto isSettled(const graph::Node& n)
        -> std::optional<bool>;

    auto resetSettlements(const std::vector<graph::Node>& touched)
        -> void;

private:
    std::vector<bool> settled_;
};


} // namespace pathfinding
