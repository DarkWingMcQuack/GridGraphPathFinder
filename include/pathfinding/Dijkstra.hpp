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

class Dijkstra
{
public:
    static constexpr auto is_thread_save = false;

    Dijkstra(const graph::GridGraph& graph) noexcept;
    Dijkstra() = delete;
    Dijkstra(Dijkstra&&) = default;
    Dijkstra(const Dijkstra&) = default;
    auto operator=(const Dijkstra&) -> Dijkstra& = delete;
    auto operator=(Dijkstra&&) -> Dijkstra& = delete;

    [[nodiscard]] auto findAllRoutes(graph::Node source, graph::Node target) noexcept
        -> std::vector<Path>;

    [[nodiscard]] auto findRoute(graph::Node source, graph::Node target) noexcept
        -> std::optional<Path>;

    [[nodiscard]] auto findDistance(graph::Node source, graph::Node target) noexcept
        -> graph::Distance;

    [[nodiscard]] auto findTrivialDistance(graph::Node source, graph::Node target) noexcept
        -> graph::Distance;

    [[nodiscard]] auto getNodesWithMinDistanceIn(const graph::GridCell& cell) noexcept
        -> std::vector<graph::Node>;

    [[nodiscard]] auto getMinDistanceIn(const graph::GridCell& cell) noexcept
        -> graph::Distance;

    [[nodiscard]] auto getGraph() const noexcept
        -> const graph::GridGraph&;

protected:
    [[nodiscard]] auto getDistanceTo(graph::Node n) const noexcept
        -> graph::Distance;

    auto setDistanceTo(graph::Node n, std::int64_t distance) noexcept
        -> void;

    [[nodiscard]] auto computeDistance(graph::Node source, graph::Node target) noexcept
        -> graph::Distance;

    [[nodiscard]] auto extractAllShortestPaths(graph::Node source, graph::Node target) const noexcept
        -> std::vector<Path>;

    [[nodiscard]] auto extractShortestPath(graph::Node source, graph::Node target) const noexcept
        -> std::optional<Path>;

    [[nodiscard]] auto findSmallestDistance(const std::vector<graph::Node>& nodes) const noexcept
        -> graph::Distance;

    [[nodiscard]] auto getIndex(graph::Node n) const noexcept
        -> std::optional<std::size_t>;

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
    DijkstraQueue pq_;
    std::optional<graph::Node> last_source_;
    std::vector<graph::Node> before_;
};


} // namespace pathfinding
