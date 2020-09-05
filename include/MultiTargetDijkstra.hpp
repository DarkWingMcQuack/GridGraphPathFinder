#pragma once

#include <GridGraph.hpp>
#include <Path.hpp>
#include <SimpleDijkstra.hpp>
#include <functional>
#include <optional>
#include <string_view>
#include <vector>

namespace pathfinding {

class MultiTargetDijkstra : public SimpleDijkstra
{
public:
    MultiTargetDijkstra(const graph::GridGraph& graph);
    MultiTargetDijkstra() = delete;
    MultiTargetDijkstra(MultiTargetDijkstra&&) = default;
    MultiTargetDijkstra(const MultiTargetDijkstra&) = delete;
    auto operator=(const MultiTargetDijkstra&) -> MultiTargetDijkstra& = delete;
    auto operator=(MultiTargetDijkstra &&) -> MultiTargetDijkstra& = delete;

    [[nodiscard]] auto findRoutes(const graph::Node& source,
                                  const std::vector<graph::Node>& targets) noexcept
        -> std::vector<std::vector<Path>>;

    [[nodiscard]] auto findDistances(const graph::Node& source,
                                     const std::vector<graph::Node>& targets) noexcept
        -> std::vector<Distance>;

private:
    [[nodiscard]] auto computeDistances(const graph::Node& source,
                                        const std::vector<graph::Node>& targets) noexcept
        -> std::vector<graph::Node>;

    auto settle(const graph::Node& n) noexcept
        -> void;

    auto unSettle(const graph::Node& n) noexcept
        -> void;

    [[nodiscard]] auto isSettled(const graph::Node& n) noexcept
        -> std::optional<bool>;

    auto resetSettlements(const std::vector<graph::Node>& touched) noexcept
        -> void;

private:
    std::vector<bool> settled_;
};


} // namespace pathfinding
