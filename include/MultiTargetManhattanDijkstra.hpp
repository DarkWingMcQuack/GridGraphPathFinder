#pragma once

#include <GridGraph.hpp>
#include <ManhattanDijkstra.hpp>
#include <SimpleDijkstra.hpp>
#include <Path.hpp>
#include <functional>
#include <optional>
#include <string_view>
#include <vector>

namespace pathfinding {

class MultiTargetManhattanDijkstra : public ManhattanDijkstra
{
public:
    MultiTargetManhattanDijkstra(const graph::GridGraph& graph);
    MultiTargetManhattanDijkstra() = delete;
    MultiTargetManhattanDijkstra(MultiTargetManhattanDijkstra&&) = default;
    MultiTargetManhattanDijkstra(const MultiTargetManhattanDijkstra&) = delete;
    auto operator=(const MultiTargetManhattanDijkstra&) -> MultiTargetManhattanDijkstra& = delete;
    auto operator=(MultiTargetManhattanDijkstra &&) -> MultiTargetManhattanDijkstra& = delete;

    template<class Iterable>
    [[nodiscard]] auto findRoutes(const graph::Node& source,
                                  const Iterable& targets) noexcept
        -> std::vector<std::vector<Path>>
    {
        auto touched = computeDistances(source, targets);

        std::vector<std::vector<Path>> paths;
        std::transform(std::cbegin(targets),
                       std::cend(targets),
                       std::back_inserter(paths),
                       [&](const auto& target) {
                           return extractShortestPaths(source, target);
                       });

        resetDistances(touched);
        resetSettlements(touched);

        return paths;
    }

    template<class Iterable>
    [[nodiscard]] auto findDistances(const graph::Node& source,
                                     const Iterable& targets) noexcept
        -> std::vector<Distance>
    {
        auto touched = computeDistances(source, targets);


        std::vector<Distance> distances;
        std::transform(std::cbegin(targets),
                       std::cend(targets),
                       std::back_inserter(distances),
                       [&](const auto& target) {
                           return getDistanceTo(target);
                       });

        resetDistances(touched);
        resetSettlements(touched);

        return distances;
    }

private:
    template<class Iterable>
    [[nodiscard]] auto computeDistances(const graph::Node& source,
                                        const Iterable& targets) noexcept
        -> std::vector<graph::Node>
    {
        using graph::Node;

        DijkstraQueue queue(DIJKSTRA_QUEUE_COMPERATOR);
        queue.emplace(source, 0l);
        setDistanceTo(source, 0l);

        std::vector<Node> touched;
        std::size_t settled_targets{0};

        while(!queue.empty()) {
            auto [current_node, current_dist] = queue.top();
            queue.pop();

            auto is_already_settled_opt = isSettled(current_node);
            if( //if the current node is a target
                std::find(std::cbegin(targets),
                          std::cend(targets),
                          current_node)
                    != std::cend(targets)
                //which was not settled before
                && is_already_settled_opt
                && !is_already_settled_opt.value()) {
                settled_targets++;
            }

            if(settled_targets >= targets.size()) {
                return touched;
            }

            settle(current_node);

            auto neigbours = getWalkableManhattanNeigboursOf(current_node);

            for(auto&& neig : neigbours) {
                touched.emplace_back(neig);

                auto neig_dist = getDistanceTo(neig);

                if(neig_dist > current_dist + 1) {
                    setDistanceTo(neig, current_dist + 1);
                    queue.emplace(neig, current_dist + 1);
                }
            }
        }

        return touched;
    }

    auto settle(const graph::Node& n)
        -> void;

    auto unSettle(const graph::Node& n)
        -> void;

    auto isSettled(const graph::Node& n)
        -> std::optional<bool>;

    auto resetSettlements(const std::vector<graph::Node>& touched)
        -> void;

private:
    std::vector<bool> settled_;
};


} // namespace pathfinding
