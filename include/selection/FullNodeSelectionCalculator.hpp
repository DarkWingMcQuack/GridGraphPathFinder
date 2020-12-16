#pragma once

#include <fmt/core.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <pathfinding/Distance.hpp>
#include <queue>
#include <random>
#include <selection/NodeSelection.hpp>
#include <selection/NodeSelectionCalculator.hpp>
#include <unordered_set>
#include <vector>

namespace selection {

template<class PathFinder>
class FullNodeSelectionCalculator
{
public:
    FullNodeSelectionCalculator(const graph::GridGraph& graph)
        : graph_(graph),
          all_to_all_(graph.size(), std::vector(graph.size(), true)),
          done_(graph.size(), true),
          node_selector_(graph)
    {
        for(auto first : graph) {
            auto first_id = graph.nodeToIndex(first);
            done_[first_id] = false;
            for(auto second : graph) {
                auto second_id = graph.nodeToIndex(second);
                if(first == second or graph.areNeighbours(first, second)) {
                    continue;
                }
                all_to_all_[first_id][second_id] = false;
            }
        }
    }

    [[nodiscard]] auto calculateFullNodeSelection() noexcept
        -> std::vector<NodeSelection>
    {
        std::vector<NodeSelection> calculated_selections;

        while(std::any_of(
            std::begin(done_),
            std::end(done_),
            [](auto x) { return !x; })) {

            auto [first, second] = getRandomRemainingPair();
            auto selection = node_selector_.calculateFullSelection(first, second).value();
            eraseNodeSelection(selection);
            calculated_selections.emplace_back(std::move(selection));
        }

        return calculated_selections;
    }

    [[nodiscard]] auto getRandomRemainingPair() const noexcept
        -> std::pair<graph::Node, graph::Node>
    {
        std::size_t first_idx;
        for(std::size_t i = 0; i < done_.size(); i++) {
            if(!done_[i]) {
                first_idx = i;
                break;
            }
        }

        std::size_t second_idx;
        for(std::size_t i = all_to_all_[first_idx].size() - 1; i >= 0; i--) {
            if(!all_to_all_[first_idx][i]) {
                second_idx = i;
                break;
            }
        }

        return std::pair{graph_.indexToNode(first_idx),
                         graph_.indexToNode(second_idx)};
    }

    auto eraseNodeSelection(const NodeSelection& selection) noexcept
        -> void
    {
        for(auto first : selection.getLeftSelection()) {
            for(auto second : selection.getRightSelection()) {
                auto first_idx = graph_.nodeToIndex(first);
                auto second_idx = graph_.nodeToIndex(second);

                all_to_all_[first_idx][second_idx] = true;
                all_to_all_[second_idx][first_idx] = true;
            }
        }

        for(auto n : selection.getLeftSelection()) {
            auto idx = graph_.nodeToIndex(n);

            done_[idx] = std::all_of(std::begin(all_to_all_[idx]),
                                     std::end(all_to_all_[idx]),
                                     [](auto x) { return x; });
        }

        for(auto n : selection.getRightSelection()) {
            auto idx = graph_.nodeToIndex(n);

            done_[idx] = std::all_of(std::begin(all_to_all_[idx]),
                                     std::end(all_to_all_[idx]),
                                     [](auto x) { return x; });
        }
    }

private:
    const graph::GridGraph& graph_;

    std::vector<std::vector<bool>> all_to_all_;
    std::vector<bool> done_;

    NodeSelectionCalculator<PathFinder> node_selector_;
};

} // namespace selection
