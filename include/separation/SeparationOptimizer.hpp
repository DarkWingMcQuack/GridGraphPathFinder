#pragma once

#include <execution>
#include <graph/GridGraph.hpp>
#include <progresscpp/ProgressBar.hpp>
#include <separation/Separation.hpp>
#include <separation/WellSeparationCalculatorCache.hpp>
#include <separation/WellSeparationChecker.hpp>
#include <unordered_set>
#include <vector>

namespace separation {

template<class PathFinder>
class SeparationOptimizer
{
public:
    static_assert(PathFinder::is_thread_save, "the path finder needs to be threadsave");

    SeparationOptimizer(const graph::GridGraph& graph,
                        const PathFinder& path_finder,
                        WellSeparationCalculatorCache cache)
        : graph_(std::move(graph)),
          path_finder_(std::move(path_finder)),
          calc_mem_(std::move(cache)) {}

    auto optimizeAll(std::vector<Separation> seps) noexcept
        -> std::vector<Separation>
    {
        separations_ = std::move(seps);

        fmt::print("optimizing the separations...\n");
        progresscpp::ProgressBar bar{separations_.size(), 80ul};

        while(!separations_.empty()) {
            auto sep = separations_.back();
            auto before = separations_.size();
            separations_.pop_back();

            auto optimal = findOptimalSeparation(sep);

            insertNewSeparation(optimal);
            auto after = separations_.size();

            bar += before - after;
            bar.displayIfChangedAtLeast(0.01);
        }

        bar.done();

        auto result = std::move(optimized_);
        optimized_.clear();

        return result;
    }

private:
    auto findOptimalSeparation(Separation current_optimum) noexcept
        -> Separation
    {
        auto left = current_optimum.getFirstCluster();
        auto right = current_optimum.getSecondCluster();

        auto current_optimal_weight = current_optimum.weight();

        auto [all_left_clusters, all_right_clusters] =
            graph_.getAllPossibleSeparationCells(left, right);

        for(auto left_cluster : all_left_clusters) {
            for(auto right_cluster : all_right_clusters) {

                if(current_optimal_weight >= weight(left_cluster, right_cluster)) {
                    continue;
                }

                if(calc_mem_.checkAndMarkCalculation(left_cluster, right_cluster)) {
                    continue;
                }


                if(auto separation = checkSeparation(path_finder_, left_cluster, right_cluster)) {
                    current_optimum = separation.value();
                    current_optimal_weight = current_optimum.weight();
                }
            }
        }

        return current_optimum;
    }

    auto insertNewSeparation(Separation sep) noexcept
        -> void
    {
        separations_.erase(
            std::remove_if(std::begin(separations_),
                           std::end(separations_),
                           [&](auto elem) {
                               return sep.isSuperSetOf(elem);
                           }),
            std::end(separations_));

        optimized_.emplace_back(sep);
    }

    auto isAlreadySolved(graph::Node from, graph::Node to)
    {
        return std::any_of(std::begin(optimized_),
                           std::end(optimized_),
                           [&](auto sep) {
                               return sep.canAnswer(from, to);
                           });
    }

    auto weight(graph::GridCell first, graph::GridCell second) const noexcept
        -> std::size_t
    {
        return std::count_if(std::begin(first),
                             std::end(first),
                             [&](auto node) {
                                 return graph_.isWalkableNode(node);
                             })
            * std::count_if(std::begin(second),
                            std::end(second),
                            [&](auto node) {
                                return graph_.isWalkableNode(node);
                            });
    }

    auto weight(const Separation& sep) const noexcept
        -> std::size_t
    {
        auto first = sep.getFirstCluster();
        auto second = sep.getSecondCluster();

        return weight(first, second);
    }


private:
    std::vector<Separation> separations_;
    std::vector<Separation> optimized_;
    const graph::GridGraph& graph_;
    const PathFinder& path_finder_;
    WellSeparationCalculatorCache calc_mem_;
};

} // namespace separation
