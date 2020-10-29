#pragma once

#include <graph/GridGraph.hpp>
#include <separation/Separation.hpp>
#include <separation/WellSeparationChecker.hpp>
#include <vector>

namespace separation {


template<class PathFinder>
class SeparationOptimizer
{
public:
    SeparationOptimizer(const graph::GridGraph& graph,
                        PathFinder path_finder)
        : graph_(std::move(graph)),
          path_finder_(std::move(path_finder)) {}

    SeparationOptimizer(const graph::GridGraph& graph)
        : graph_(std::move(graph)),
          path_finder_(PathFinder{graph}) {}

    auto checkIfBetterSeparationExists(const Separation& sep) const noexcept
        -> bool
    {
        auto optimized = findOptimalSeparation(sep);

        return getFirstCluster(sep).size() * getSecondCluster(sep).size()
            < getFirstCluster(optimized).size() * getSecondCluster(optimized).size();
    }

private:
    auto findOptimalSeparation(const Separation& sep) const noexcept
        -> Separation
    {
        auto left = getFirstCluster(sep);
        auto right = getSecondCluster(sep);

        auto left_representative = left[0];
        auto right_representative = right[0];

        auto all_left_clusters = graph_.getAllCellsContaining(left_representative);
        auto all_right_clusters = graph_.getAllCellsContaining(right_representative);

        std::vector<Separation> found_separations;

        for(auto left_cluster : all_left_clusters) {
            for(auto right_cluster : all_right_clusters) {
                if(auto separation = checkSeparation(path_finder_, left_cluster, right_cluster)) {
				  found_separations.emplace_back(separation.value());
                }
            }
        }

        return *std::max_element(std::begin(found_separations),
                                 std::end(found_separations),
                                 [](auto lhs, auto rhs) {
                                     return getFirstCluster(lhs).size() * getSecondCluster(lhs).size()
                                         < getFirstCluster(rhs).size() * getSecondCluster(rhs).size();
                                 });
    }

    auto insertNewSeparation(Separation sep) noexcept
        -> void
    {
        std::copy_if(std::begin(separations_),
                     std::end(separations_),
                     std::back_inserter(allready_optimized_),
                     [&](auto elem) {
                         return isSuperSetOf(sep, elem);
                     });

        separations_.erase(std::remove_if(std::begin(separations_),
                                          std::end(separations_),
                                          [&](auto elem) {
                                              return isSuperSetOf(sep, elem);
                                          }),
                           std::end(separations_));

        separations_.emplace_back(sep);
    }

private:
    std::vector<Separation> separations_;
    std::vector<Separation> allready_optimized_;
    const graph::GridGraph& graph_;
    mutable PathFinder path_finder_;
};

} // namespace separation
