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

class CachingDijkstra
{
public:
    static constexpr auto is_thread_save = true;

    CachingDijkstra(const graph::GridGraph &graph) noexcept;
    CachingDijkstra() = delete;
    CachingDijkstra(CachingDijkstra &&) = default;
    CachingDijkstra(const CachingDijkstra &) = default;
    auto operator=(const CachingDijkstra &) -> CachingDijkstra & = delete;
    auto operator=(CachingDijkstra &&) -> CachingDijkstra & = delete;

    [[nodiscard]] auto findDistance(const graph::Node &source,
                                    const graph::Node &target) const noexcept
        -> graph::Distance;

    [[nodiscard]] auto findTrivialDistance(const graph::Node &source,
                                           const graph::Node &target) const noexcept
        -> graph::Distance;

    [[nodiscard]] auto getGraph() const noexcept
        -> const graph::GridGraph &;

    auto destroy() noexcept -> void;

private:
    [[nodiscard]] auto getDistanceTo(const graph::Node &n) const noexcept
        -> graph::Distance;

    auto setDistanceTo(const graph::Node &n, std::int64_t distance) noexcept
        -> void;

    [[nodiscard]] auto computeDistance(const graph::Node &source,
                                       const graph::Node &target) noexcept
        -> graph::Distance;

    [[nodiscard]] auto getIndex(const graph::Node &n) const noexcept
        -> std::optional<std::size_t>;

    auto unSettle(const graph::Node &n) noexcept -> void;

    auto settle(const graph::Node &n) noexcept -> void;

    [[nodiscard]] auto isSettled(const graph::Node &n) noexcept -> bool;

    auto reset() noexcept -> void;

    auto insertCache(graph::Node first, graph::Node second, graph::Distance dist) noexcept
        -> void;

    [[nodiscard]] auto queryCache(graph::Node first, graph::Node second) const noexcept
        -> graph::Distance;

    [[nodiscard]] auto getWalkableNeigboursOf(graph::Node node) const noexcept
        -> std::vector<graph::Node>;

private:
    const std::reference_wrapper<const graph::GridGraph> graph_;
    std::vector<graph::Distance> distances_;
    std::vector<bool> settled_;
    std::vector<graph::Node> touched_;
    DijkstraQueue pq_;
    std::optional<graph::Node> last_source_;

    std::vector<std::size_t> cache_index_;
    using DistanceCache = std::vector<std::vector<graph::Distance>>;
    DistanceCache distance_cache_;
};

} // namespace pathfinding
