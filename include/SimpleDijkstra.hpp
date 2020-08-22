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

private:
    auto getDistanceTo(const graph::Node& n) const
        -> std::optional<std::int64_t>;

    auto setDistanceTo(const graph::Node& n, std::int64_t distance)
        -> void;

    auto computeDistances(const graph::Node& source, const graph::Node& target)
        -> std::vector<graph::Node>;

    auto extractShortestPaths(const graph::Node& source, const graph::Node& target) const
        -> std::vector<Path>;

    auto resetDistances(const std::vector<graph::Node>& touched)
        -> void;

    auto findSmallestDistance(const std::vector<graph::Node>& nodes) const
        -> std::optional<std::int64_t>;

private:
    const std::reference_wrapper<const graph::GridGraph> graph_;
    std::vector<std::int64_t> distances_;
};


} // namespace pathfinding
