#pragma once

#include <GridGraph.hpp>
#include <Path.hpp>
#include <functional>
#include <optional>
#include <string_view>
#include <vector>

namespace pathfinding {

class SimpleDijkstra
{
public:
    SimpleDijkstra(const graph::GridGraph& graph) noexcept;
    SimpleDijkstra() = delete;
    SimpleDijkstra(SimpleDijkstra&&) = default;
    SimpleDijkstra(const SimpleDijkstra&) = delete;
    auto operator=(const SimpleDijkstra&) -> SimpleDijkstra& = delete;
    auto operator=(SimpleDijkstra &&) -> SimpleDijkstra& = delete;

    auto findRoutes(const graph::Node& source, const graph::Node& target) noexcept
        -> std::vector<Path>;

protected:
    auto getDistanceTo(const graph::Node& n) const noexcept
        -> Distance;

    auto setDistanceTo(const graph::Node& n, std::int64_t distance) noexcept
        -> void;

    auto computeDistances(const graph::Node& source, const graph::Node& target) noexcept
        -> std::vector<graph::Node>;

    auto extractShortestPaths(const graph::Node& source, const graph::Node& target) const noexcept
        -> std::vector<Path>;

    auto resetDistances(const std::vector<graph::Node>& touched) noexcept
        -> void;

    auto findSmallestDistanceGreaterThan(const std::vector<graph::Node>& nodes) const  noexcept
        -> Distance;

    auto getWalkableNeigboursOf(const graph::Node& n) const noexcept
        -> std::vector<graph::Node>;

    auto getIndex(const graph::Node& n) const noexcept
        -> std::optional<std::size_t>;

private:
    const std::reference_wrapper<const graph::GridGraph> graph_;
    std::vector<Distance> distances_;
};


} // namespace pathfinding
