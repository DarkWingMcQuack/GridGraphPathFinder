#pragma once

#include <functional>
#include <optional>
#include <pathfinding/DijkstraQueue.hpp>
#include <pathfinding/Distance.hpp>
#include <pathfinding/Path.hpp>
#include <queue>
#include <string_view>
#include <vector>

namespace graph {
class GridGraph;
}

namespace pathfinding {

class AStar
{
public:
    static constexpr auto is_thread_save = false;

    AStar(const graph::GridGraph& graph) noexcept;
    AStar() = delete;
    AStar(AStar&&) = default;
    AStar(const AStar&) = default;
    auto operator=(const AStar&) -> AStar& = delete;
    auto operator=(AStar&&) -> AStar& = delete;

    [[nodiscard]] auto findRoute(graph::Node source, graph::Node target) noexcept
        -> std::optional<Path>;

    [[nodiscard]] auto findDistance(graph::Node source, graph::Node target) noexcept
        -> graph::Distance;

private:
    [[nodiscard]] auto findTrivialDistance(graph::Node source, graph::Node target) noexcept
        -> graph::Distance;

    [[nodiscard]] auto getDistanceTo(graph::Node n) const noexcept
        -> graph::Distance;

    auto setDistanceTo(graph::Node n, std::int64_t distance) noexcept
        -> void;

    [[nodiscard]] auto computeDistance(graph::Node source, graph::Node target) noexcept
        -> graph::Distance;

    [[nodiscard]] auto extractShortestPath(graph::Node source, graph::Node target) const noexcept
        -> std::optional<Path>;

    auto unSettle(graph::Node n)
        -> void;

    auto settle(graph::Node n) noexcept
        -> void;

    [[nodiscard]] auto isSettled(graph::Node n)
        -> bool;

    auto reset() noexcept
        -> void;

    auto setBefore(graph::Node n, graph::Node before) noexcept
        -> void;

private:
    const std::reference_wrapper<const graph::GridGraph> graph_;
    std::vector<graph::Distance> distances_;
    std::vector<bool> settled_;
    std::vector<graph::Node> touched_;
    AStarQueue pq_;
    std::optional<graph::Node> last_source_;
    std::vector<graph::Node> before_;
};


} // namespace pathfinding
