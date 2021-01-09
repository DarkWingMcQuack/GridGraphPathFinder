
#pragma once

#include <cmath>
#include <fmt/core.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <pathfinding/DijkstraQueue.hpp>
#include <pathfinding/Distance.hpp>
#include <queue>
#include <selection/NodeSelection.hpp>
#include <vector>

namespace selection {

template<class PathFinder>
class SelectionCenterCalculator
{
public:
    SelectionCenterCalculator(const graph::GridGraph& graph)
        : graph_(graph),
          path_finder_(graph_),
          settled_(graph_.size(), false) {}

    auto calculateCenter(graph::Node from, graph::Node to) noexcept
        -> std::optional<graph::Node>
    {
        auto path_opt = getPath(from, to);
        if(!path_opt) {
            return std::nullopt;
        }

        auto path = std::move(path_opt.value());
        auto initial_center_opt = findCenter(path);
        if(!initial_center_opt) {
            return std::nullopt;
        }

        auto initial_center = std::move(initial_center_opt.value());
        // auto optimization_range = static_cast<graph::Distance>(std::floor(path.getLength() / 4.0));
        // auto optimized = optimzeCenter(from,
        //                                to,
        //                                initial_center,
        //                                path.getLength(),
        //                                optimization_range);
        // reset();

        return initial_center;
    }

private:
    auto getPath(graph::Node from, graph::Node to) noexcept
        -> std::optional<pathfinding::Path>
    {
        return path_finder_.findRoute(from, to);
    }

    auto findCenter(const pathfinding::Path& path) const noexcept
        -> std::optional<graph::Node>
    {
        return path.getMiddleNode();
    }

    auto optimzeCenter(graph::Node source,
                       graph::Node target,
                       graph::Node initial_center,
                       graph::Distance source_target_dist,
                       graph::Distance optimization_range) noexcept
        -> graph::Node
    {
        pq_.emplace(initial_center, 0ul);

        while(!pq_.empty()) {
            const auto [current_center, current_dist] = pq_.top();
            pq_.pop();

            auto current_idx = graph_.nodeToIndex(current_center);
            settled_[current_idx] = true;

            if(current_dist > optimization_range) {
                return initial_center;
            }

            if(graph_.hasBarrierNeigbour(current_center)) {
                return current_center;
            }

            auto neigbours = graph_.getManhattanNeigbours(current_center);

            for(auto neig : neigbours) {
                auto neig_idx = graph_.nodeToIndex(neig);
                if(graph_.isBarrier(neig)) {
                    continue;
                }

                auto src_neig = path_finder_.findDistance(neig, source);
                auto tar_neig = path_finder_.findDistance(neig, target);
                auto neig_dist = current_dist + 1;

                if(src_neig + tar_neig == source_target_dist
                   and !settled_[neig_idx]) {
                    touched_.emplace_back(neig_idx);
                    pq_.emplace(neig, neig_dist);
                }
            }
        }

        return initial_center;
    }

    auto reset() noexcept
        -> void
    {
        for(auto i : touched_) {
            settled_[i] = false;
        }

        touched_.clear();
        pq_ = pathfinding::DijkstraQueue{pathfinding::DijkstraQueueComparer{}};
    }

private:
    const graph::GridGraph& graph_;
    PathFinder path_finder_;
    std::vector<bool> settled_;
    std::vector<std::size_t> touched_;
    pathfinding::DijkstraQueue pq_;
};

} // namespace selection
