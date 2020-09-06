#pragma once

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

class ManhattanDijkstra
{
public:
    ManhattanDijkstra(const graph::GridGraph& graph) noexcept;
    ManhattanDijkstra() = delete;
    ManhattanDijkstra(ManhattanDijkstra&&) = default;
    ManhattanDijkstra(const ManhattanDijkstra&) = delete;
    auto operator=(const ManhattanDijkstra&) -> ManhattanDijkstra& = delete;
    auto operator=(ManhattanDijkstra &&) -> ManhattanDijkstra& = delete;

    auto findRoutes(const graph::Node& source, const graph::Node& target) noexcept
        -> std::vector<Path>;

    auto findDistance(const graph::Node& source, const graph::Node& target) noexcept
        -> graph::Distance;

protected:
    auto getDistanceTo(const graph::Node& n) const noexcept
        -> graph::Distance;

    auto setDistanceTo(const graph::Node& n, std::int64_t distance) noexcept
        -> void;

    auto computeDistances(const graph::Node& source, const graph::Node& target) noexcept
        -> std::vector<graph::Node>;

    auto extractShortestPaths(const graph::Node& source, const graph::Node& target) const noexcept
        -> std::vector<Path>;

    auto resetDistances(const std::vector<graph::Node>& touched) noexcept
        -> void;

    auto findSmallestDistance(const std::vector<graph::Node>& nodes) const noexcept
        -> graph::Distance;

    auto getWalkableManhattanNeigboursOf(const graph::Node& n) const noexcept
        -> std::vector<graph::Node>;

    auto getIndex(const graph::Node& n) const noexcept
        -> std::optional<std::size_t>;

private:
    const std::reference_wrapper<const graph::GridGraph> graph_;
    std::vector<graph::Distance> distances_;
};


} // namespace pathfinding
