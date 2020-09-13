#pragma once

#include <DijkstraQueue.hpp>
#include <Distance.hpp>
#include <Path.hpp>
#include <functional>
#include <optional>
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
    Dijkstra(const graph::GridGraph& graph) noexcept;
    Dijkstra() = delete;
    Dijkstra(Dijkstra&&) = default;
    Dijkstra(const Dijkstra&) = delete;
    auto operator=(const Dijkstra&) -> Dijkstra& = delete;
    auto operator=(Dijkstra &&) -> Dijkstra& = delete;

    [[nodiscard]] auto findRoutes(const graph::Node& source, const graph::Node& target) noexcept
        -> std::vector<Path>;

    [[nodiscard]] auto findDistance(const graph::Node& source, const graph::Node& target) noexcept
        -> graph::Distance;

    [[nodiscard]] auto getNodesWithMinDistanceIn(const grid::GridCell& cell) noexcept
        -> std::vector<graph::Node>;

    [[nodiscard]] auto getGraph() const noexcept
        -> const graph::GridGraph&;


protected:
    auto getDistanceTo(const graph::Node& n) const noexcept
        -> graph::Distance;

    auto setDistanceTo(const graph::Node& n, std::int64_t distance) noexcept
        -> void;

    auto computeDistances(const graph::Node& source, const graph::Node& target) noexcept
        -> void;

    auto extractShortestPaths(const graph::Node& source, const graph::Node& target) const noexcept
        -> std::vector<Path>;

    auto findSmallestDistance(const std::vector<graph::Node>& nodes) const noexcept
        -> graph::Distance;

    auto getWalkableManhattanNeigboursOf(const graph::Node& n) const noexcept
        -> std::vector<graph::Node>;

    auto getIndex(const graph::Node& n) const noexcept
        -> std::optional<std::size_t>;

    auto unSettle(const graph::Node& n)
        -> void;

    auto settle(const graph::Node& n) noexcept
        -> void;

    auto isSettled(const graph::Node& n)
        -> std::optional<bool>;

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
