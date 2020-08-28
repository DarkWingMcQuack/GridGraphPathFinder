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
    SimpleDijkstra(const graph::GridGraph& graph);
    SimpleDijkstra() = delete;
    SimpleDijkstra(SimpleDijkstra&&) = default;
    SimpleDijkstra(const SimpleDijkstra&) = delete;
    auto operator=(const SimpleDijkstra&) -> SimpleDijkstra& = delete;
    auto operator=(SimpleDijkstra &&) -> SimpleDijkstra& = delete;

    auto findRoutes(const graph::Node& source, const graph::Node& target)
        -> std::vector<Path>;

protected:
    auto getDistanceTo(const graph::Node& n) const
        -> Distance;

    auto setDistanceTo(const graph::Node& n, std::int64_t distance)
        -> void;

    auto computeDistances(const graph::Node& source, const graph::Node& target)
        -> std::vector<graph::Node>;

    auto extractShortestPaths(const graph::Node& source, const graph::Node& target) const
        -> std::vector<Path>;

    auto resetDistances(const std::vector<graph::Node>& touched)
        -> void;

    auto findSmallestDistanceGreaterThan(const std::vector<graph::Node>& nodes) const 
        -> Distance;

    auto getWalkableNeigboursOf(const graph::Node& n) const
        -> std::vector<graph::Node>;

    auto getIndex(const graph::Node& n) const
        -> std::optional<std::size_t>;

private:
    const std::reference_wrapper<const graph::GridGraph> graph_;
    std::vector<Distance> distances_;
};


} // namespace pathfinding
