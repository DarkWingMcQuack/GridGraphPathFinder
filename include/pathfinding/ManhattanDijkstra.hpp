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

class ManhattanDijkstra
{
public:
    ManhattanDijkstra(const graph::GridGraph& graph) noexcept;
    ManhattanDijkstra() = delete;
    ManhattanDijkstra(ManhattanDijkstra&&) = default;
    ManhattanDijkstra(const ManhattanDijkstra&) = default;
    auto operator=(const ManhattanDijkstra&) -> ManhattanDijkstra& = delete;
    auto operator=(ManhattanDijkstra &&) -> ManhattanDijkstra& = delete;

    [[nodiscard]] auto findAllRoutes(const graph::Node& source, const graph::Node& target) noexcept
        -> std::vector<Path>;

    [[nodiscard]] auto findRoute(const graph::Node& source, const graph::Node& target) noexcept
        -> std::optional<Path>;

    [[nodiscard]] auto findDistance(const graph::Node& source, const graph::Node& target) noexcept
        -> graph::Distance;

    [[nodiscard]] auto getNodesWithMinDistanceIn(const graph::GridCell& cell) noexcept
        -> std::vector<graph::Node>;

    [[nodiscard]] auto getMinDistanceIn(const graph::GridCell& cell) noexcept
        -> graph::Distance;

    [[nodiscard]] auto getGraph() const noexcept
        -> const graph::GridGraph&;

protected:
    [[nodiscard]] auto getWalkableNeigboursOf(const graph::Node& n) const noexcept
        -> std::vector<graph::Node>;

    [[nodiscard]] auto getDistanceTo(const graph::Node& n) const noexcept
        -> graph::Distance;

    auto setDistanceTo(const graph::Node& n, std::int64_t distance) noexcept
        -> void;

    [[nodiscard]] auto computeDistance(const graph::Node& source, const graph::Node& target) noexcept
        -> graph::Distance;

    [[nodiscard]] auto extractAllShortestPaths(const graph::Node& source, const graph::Node& target) const noexcept
        -> std::vector<Path>;

    [[nodiscard]] auto extractShortestPath(const graph::Node& source, const graph::Node& target) const noexcept
        -> std::optional<Path>;

    [[nodiscard]] auto findSmallestDistance(const std::vector<graph::Node>& nodes) const noexcept
        -> graph::Distance;

    [[nodiscard]] auto getIndex(const graph::Node& n) const noexcept
        -> std::optional<std::size_t>;

    auto unSettle(const graph::Node& n)
        -> void;

    auto settle(const graph::Node& n) noexcept
        -> void;

    [[nodiscard]] auto isSettled(const graph::Node& n)
        -> bool;

    auto reset() noexcept
        -> void;

private:
    const std::reference_wrapper<const graph::GridGraph> graph_;
    std::vector<graph::Distance> distances_;
    std::vector<bool> settled_;
    std::vector<graph::Node> touched_;
    DijkstraQueue pq_;
    std::optional<graph::Node> last_source_;
};


} // namespace pathfinding
