#pragma once

#include <execution>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <pathfinding/Distance.hpp>
#include <progresscpp/ProgressBar.hpp>
#include <queue>
#include <random>
#include <selection/NodeSelection.hpp>
#include <selection/NodeSelectionCalculator.hpp>
#include <vector>

namespace selection {

template<class PathFinder,
         class CachedPathFinder>
class FullNodeSelectionCalculator
{
public:
    FullNodeSelectionCalculator(const graph::GridGraph& graph)
        : graph_(graph),
          all_to_all_(graph.size()),
          node_selector_(graph)
    {
        for(auto first : graph) {
            auto first_id = graph.nodeToIndex(first);

            all_to_all_[first_id] = std::vector(graph.size(), true);

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

        progresscpp::ProgressBar bar{graph_.size(), 80ul};
        auto done_counter = countDoneNodes();

        while(std::any_of(
            std::begin(all_to_all_),
            std::end(all_to_all_),
            [](const auto& x) { return !x.empty(); })) {

            auto [first, second] = getRandomRemainingPair();
            auto selection = node_selector_.calculateFullSelection(first, second).value();
            optimizeSelection(selection);
            eraseNodeSelection(selection);
            calculated_selections.emplace_back(std::move(selection));


            //update progress bar
            auto new_done = countDoneNodes();
            auto diff = new_done - done_counter;
            bar += diff;
            done_counter = new_done;

            bar.displayIfChangedAtLeast(0.001);
        }

        bar.done();

        return calculated_selections;
    }

    [[nodiscard]] auto getRandomRemainingPair() const noexcept
        -> std::pair<graph::Node, graph::Node>
    {
        auto undone = std::count_if(std::begin(all_to_all_),
                                    std::end(all_to_all_),
                                    [](const auto& b) {
                                        return !b.empty();
                                    });

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<std::size_t> outer_dis(0, undone - 1);

        auto outer_random = outer_dis(gen);

        std::size_t first_idx;
        std::size_t counter = 0;
        for(std::size_t i = 0; i < all_to_all_.size(); i++) {
            if(!all_to_all_[i].empty()) {
                if(counter++ == outer_random) {
                    first_idx = i;
                    break;
                }
            }
        }

        auto inner_undone = std::count_if(std::begin(all_to_all_[first_idx]),
                                          std::end(all_to_all_[first_idx]),
                                          [](auto b) {
                                              return !b;
                                          });

        std::uniform_int_distribution<std::size_t> inner_dis(0, inner_undone - 1);

        auto inner_random = inner_dis(gen);

        std::size_t inner_counter = 0;
        std::size_t second_idx;
        for(std::size_t i = all_to_all_[first_idx].size() - 1; i >= 0; i--) {
            if(!all_to_all_[first_idx][i]) {
                if(inner_counter++ == inner_random) {
                    second_idx = i;
                    break;
                }
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

                if(!all_to_all_[first_idx].empty()) {
                    all_to_all_[first_idx][second_idx] = true;
                }

                if(!all_to_all_[second_idx].empty()) {
                    all_to_all_[second_idx][first_idx] = true;
                }
            }
        }

        for(auto n : selection.getLeftSelection()) {
            auto idx = graph_.nodeToIndex(n);

            auto done = std::all_of(std::begin(all_to_all_[idx]),
                                    std::end(all_to_all_[idx]),
                                    [](auto x) { return x; });

            if(done) {
                all_to_all_[idx].clear();
            }
        }

        for(auto n : selection.getRightSelection()) {
            auto idx = graph_.nodeToIndex(n);

            auto done = std::all_of(std::begin(all_to_all_[idx]),
                                    std::end(all_to_all_[idx]),
                                    [](auto x) { return x; });

            if(done) {
                all_to_all_[idx].clear();
            }
        }
    }

    auto optimizeSelection(NodeSelection& selection) noexcept
        -> void
    {
        auto changed = true;
        auto& left = selection.getLeftSelection();
        auto& right = selection.getRightSelection();
        while(changed) {
            auto weight_before = selection.weight();

            left.erase(
                std::remove_if(std::begin(left),
                               std::end(left),
                               [&](auto node) {
                                   return areAllSettledFor(right, node);
                               }),
                std::end(left));

            right.erase(
                std::remove_if(std::begin(right),
                               std::end(right),
                               [&](auto node) {
                                   return areAllSettledFor(left, node);
                               }),
                std::end(right));

            auto weight_after = selection.weight();

            changed = weight_before != weight_after;
        }
    }

    auto areAllSettledFor(const std::vector<graph::Node>& settled, graph::Node node) const noexcept
        -> bool
    {
        auto n_idx = graph_.nodeToIndex(node);
        const auto& settle_vec = all_to_all_[n_idx];

        if(settle_vec.empty()) {
            return true;
        }

        return std::all_of(std::begin(settled),
                           std::end(settled),
                           [&](auto n) {
                               auto idx = graph_.nodeToIndex(n);
                               return settle_vec[idx];
                           });
    }

    auto countDoneNodes() const noexcept
        -> std::size_t
    {
        return std::count_if(std::begin(all_to_all_),
                             std::end(all_to_all_),
                             [](const auto& b) {
                                 return b.empty();
                             });
    }

private:
    const graph::GridGraph& graph_;
    std::vector<std::vector<bool>> all_to_all_;
    NodeSelectionCalculator<PathFinder, CachedPathFinder> node_selector_;
};

} // namespace selection
