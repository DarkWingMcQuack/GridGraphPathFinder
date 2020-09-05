#pragma once

#include <GridGraph.hpp>
#include <Path.hpp>
#include <functional>
#include <optional>
#include <queue>
#include <string_view>
#include <vector>

namespace pathfinding {

constexpr auto DIJKSTRA_QUEUE_COMPERATOR = [](const auto& lhs,
                                              const auto& rhs) {
    return lhs.second < rhs.second;
};

using DijkstraQueue = std::priority_queue<std::pair<graph::Node, Distance>,
                                          std::vector<std::pair<graph::Node, Distance>>,
                                          decltype(DIJKSTRA_QUEUE_COMPERATOR)>;

class SimpleDijkstra
{
public:
    SimpleDijkstra(const graph::GridGraph& graph) noexcept;
    SimpleDijkstra() = delete;
    SimpleDijkstra(SimpleDijkstra&&) = default;
    SimpleDijkstra(const SimpleDijkstra&) = delete;
    auto operator=(const SimpleDijkstra&) -> SimpleDijkstra& = delete;
    auto operator=(SimpleDijkstra &&) -> SimpleDijkstra& = delete;

    [[nodiscard]] auto findRoutes(const graph::Node& source, const graph::Node& target) noexcept
        -> std::vector<Path>;

    [[nodiscard]] auto findDistance(const graph::Node& source, const graph::Node& target) noexcept
        -> Distance;

protected:
    [[nodiscard]] auto getDistanceTo(const graph::Node& n) const noexcept
        -> Distance;

    auto setDistanceTo(const graph::Node& n, std::int64_t distance) noexcept
        -> void;

    [[nodiscard]] auto computeDistances(const graph::Node& source, const graph::Node& target) noexcept
        -> std::vector<graph::Node>;

    [[nodiscard]] auto extractShortestPaths(const graph::Node& source, const graph::Node& target) const noexcept
        -> std::vector<Path>;

    auto resetDistances(const std::vector<graph::Node>& touched) noexcept
        -> void;

    [[nodiscard]] auto findSmallestDistance(const std::vector<graph::Node>& nodes) const noexcept
        -> Distance;

    [[nodiscard]] auto getWalkableNeigboursOf(const graph::Node& n) const noexcept
        -> std::vector<graph::Node>;

    [[nodiscard]] auto getIndex(const graph::Node& n) const noexcept
        -> std::optional<std::size_t>;

private:
    const std::reference_wrapper<const graph::GridGraph> graph_;
    std::vector<Distance> distances_;
};


} // namespace pathfinding
