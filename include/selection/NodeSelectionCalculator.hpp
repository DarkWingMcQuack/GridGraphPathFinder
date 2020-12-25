
#pragma once

#include <execution>
#include <fmt/core.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <pathfinding/Distance.hpp>
#include <queue>
#include <selection/NodeSelection.hpp>
#include <vector>

namespace selection {

template<class PathFinder,
         class CachedPathFinder>
class NodeSelectionCalculator
{
public:
    NodeSelectionCalculator(const graph::GridGraph& graph)
        : path_finder_(graph),
          cached_path_finder_(graph),
          graph_(graph),
          left_settled_(graph_.size(), false),
          right_settled_(graph_.size(), false) {}


    [[nodiscard]] auto calculateFullSelection(graph::Node left_start,
                                              graph::Node right_start) noexcept
        -> std::optional<NodeSelection>
    {
        auto center_opt = calculateCenter(left_start, right_start);
        if(!center_opt) {
            return std::nullopt;
        }
        auto center = center_opt.value();

        auto left_to_center = cached_path_finder_.findDistance(left_start, center);

        std::deque<graph::Node> left_candidates;
        left_candidates.push_back(left_start);
        left_selection_.emplace_back(left_start, left_to_center);

        auto right_to_center = cached_path_finder_.findDistance(right_start, center);

        std::deque<graph::Node> right_candidates;
        right_candidates.push_back(right_start);
        right_selection_.emplace_back(right_start, right_to_center);


        while(!left_candidates.empty() or !right_candidates.empty()) {

            if(!left_candidates.empty()) {
                auto current = left_candidates.front();
                left_candidates.pop_front();

                if(auto left_dist_opt = checkLeftAffiliation(current, center)) {
                    auto left_dist = left_dist_opt.value();
                    left_selection_.emplace_back(current, left_dist);

                    for(auto neig : graph_.getWalkableNeigbours(current)) {
                        if(!isLeftSettled(neig)) {
                            settleLeft(neig);
                            touched_.push_back(neig);
                            left_candidates.push_back(neig);
                        }
                    }
                }
            }

            if(!right_candidates.empty()) {
                auto current = right_candidates.front();
                right_candidates.pop_front();

                if(auto right_dist_opt = checkRightAffiliation(current, center)) {
                    auto right_dist = right_dist_opt.value();
                    right_selection_.emplace_back(current, right_dist);

                    for(auto neig : graph_.getWalkableNeigbours(current)) {
                        if(!isRightSettled(neig)) {
                            settleRight(neig);
                            touched_.push_back(neig);
                            right_candidates.push_back(neig);
                        }
                    }
                }
            }
        }

        //create the selection which was found
        std::vector<graph::Node> left;
        std::transform(std::begin(left_selection_),
                       std::end(left_selection_),
                       std::back_inserter(left),
                       [](auto pair) {
                           return pair.first;
                       });

        std::vector<graph::Node> right;
        std::transform(std::begin(right_selection_),
                       std::end(right_selection_),
                       std::back_inserter(right),
                       [](auto pair) {
                           return pair.first;
                       });

        NodeSelection selection{std::move(left),
                                std::move(right),
                                center};

        //cleanup and reset the state of the calculator
        cleanup();

        return selection;
    }


    [[nodiscard]] auto checkLeftAffiliation(const graph::Node& node,
                                            const graph::Node& center) noexcept
        -> std::optional<graph::Distance>
    {
        return checkAffiliation(node, center, right_selection_);
    }

    [[nodiscard]] auto checkRightAffiliation(const graph::Node& node,
                                             const graph::Node& center) noexcept
        -> std::optional<graph::Distance>
    {
        return checkAffiliation(node, center, left_selection_);
    }


    [[nodiscard]] auto checkAffiliation(const graph::Node& node,
                                        const graph::Node& center,
                                        const std::vector<std::pair<graph::Node, graph::Distance>>& selection) noexcept
        -> std::optional<graph::Distance>
    {
        auto center_dist = cached_path_finder_.findDistance(node, center);

        auto valid = std::all_of(std::execution::unseq,
                                 std::begin(selection),
                                 std::end(selection),
                                 [&](auto pair) {
                                     auto [target, center_target_dist] = pair;

                                     auto dist = cached_path_finder_.findDistance(node, target);

                                     if(center_target_dist == graph::UNREACHABLE
                                        or center_dist == graph::UNREACHABLE) {
                                         if(dist != graph::UNREACHABLE) {
                                             return false;
                                         }
                                     }

                                     return center_dist + center_target_dist == dist;
                                 });

        if(!valid) {
            return std::nullopt;
        }

        return center_dist;
    }

private:
    auto cleanup() noexcept
        -> void
    {
        for(auto n : touched_) {
            unsettle(n);
        }
        touched_.clear();
        left_selection_.clear();
        right_selection_.clear();
    }

    auto unsettle(const graph::Node& node) noexcept
        -> void
    {
        auto index = graph_.nodeToIndex(node);
        left_settled_[index] = false;
        right_settled_[index] = false;
    }

    auto settleLeft(const graph::Node& node) noexcept
        -> void
    {
        auto index = graph_.nodeToIndex(node);
        left_settled_[index] = true;
    }

    auto settleRight(const graph::Node& node) noexcept
        -> void
    {
        auto index = graph_.nodeToIndex(node);
        right_settled_[index] = true;
    }

    [[nodiscard]] auto isLeftSettled(const graph::Node& node) const noexcept
        -> bool
    {
        auto index = graph_.nodeToIndex(node);
        return left_settled_[index];
    }

    [[nodiscard]] auto isRightSettled(const graph::Node& node) const noexcept
        -> bool
    {
        auto index = graph_.nodeToIndex(node);
        return right_settled_[index];
    }

    [[nodiscard]] auto calculateCenter(const graph::Node& left,
                                       const graph::Node& right) noexcept
        -> std::optional<graph::Node>
    {

        auto left_to_right_path_opt = path_finder_.findRoute(left, right);
        if(!left_to_right_path_opt) {
            return std::nullopt;
        }
        return left_to_right_path_opt.value().getMiddleNode();
    }

private:
    PathFinder path_finder_;
    CachedPathFinder cached_path_finder_;
    const graph::GridGraph& graph_;

    std::vector<graph::Node> touched_;

    std::vector<bool> left_settled_;
    std::vector<bool> right_settled_;

    std::vector<std::pair<graph::Node, graph::Distance>> left_selection_;
    std::vector<std::pair<graph::Node, graph::Distance>> right_selection_;
};

} // namespace selection
