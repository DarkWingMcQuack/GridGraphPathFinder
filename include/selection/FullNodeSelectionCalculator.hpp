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
        : node_selector_(graph)
    {
        for(auto first : graph) {
            for(auto second : graph) {
                if(first == second or graph.areNeighbours(first, second)) {
                    continue;
                }
                all_to_all_pairs_.emplace(std::pair{first, second});
            }
        }
    }

    [[nodiscard]] auto calculateFullNodeSelection() noexcept
        -> std::vector<NodeSelection>
    {
        std::vector<NodeSelection> calculated_selections;
        while(!all_to_all_pairs_.empty()) {
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
        static std::mt19937 engine{static_cast<std::bernoulli_distribution::result_type>(time(nullptr))};
        std::uniform_int_distribution<std::size_t> dist(0, all_to_all_pairs_.size() - 1);

        auto iter = std::begin(all_to_all_pairs_);
        std::advance(iter, dist(engine));

        return *iter;
    }

    auto eraseNodeSelection(const NodeSelection& selection) noexcept
        -> void
    {
        for(auto first : selection.getLeftSelection()) {
            for(auto second : selection.getRightSelection()) {
                all_to_all_pairs_.erase(std::pair{first, second});
                all_to_all_pairs_.erase(std::pair{second, first});
            }
        }

        fmt::print("remaining size: {}\n",
                   all_to_all_pairs_.size());
    }

private:
    std::unordered_set<std::pair<graph::Node,
                                 graph::Node>>
        all_to_all_pairs_;

    NodeSelectionCalculator<PathFinder> node_selector_;
};

} // namespace selection
