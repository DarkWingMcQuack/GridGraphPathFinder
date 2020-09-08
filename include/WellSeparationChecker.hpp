#pragma once


#include <Distance.hpp>
#include <GridCell.hpp>
#include <GridGraph.hpp>
#include <PathQuerySystem.hpp>
#include <Separation.hpp>
#include <SimpleDijkstra.hpp>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <future>
#include <iterator>
#include <numeric>
#include <vector>

namespace separation {

template<class MultiTargetPathFinder>
[[nodiscard]] auto findCenterCandidates(const pathfinding::PathQuerySystem<MultiTargetPathFinder>& multi_path_finder,
                                        const grid::GridCell& first,
                                        const grid::GridCell& second) noexcept
    -> std::optional<std::tuple<graph::Node, graph::Node, graph::Distance>>
{
    using graph::Distance;
    using graph::UNREACHABLE;

    std::vector<std::future<std::vector<Distance>>> distance_futs;
    distance_futs.reserve(first.size());
    std::transform(std::begin(first),
                   std::end(first),
                   std::back_inserter(distance_futs),
                   [&](auto source) {
                       return multi_path_finder.query(
                           source,
                           second);
                   });

    if(distance_futs.empty()) {
        return std::nullopt;
    }

    std::vector<std::vector<Distance>> distances;
    distances.reserve(first.size());
    std::transform(std::make_move_iterator(std::begin(distance_futs)),
                   std::make_move_iterator(std::end(distance_futs)),
                   std::back_inserter(distances),
                   [](auto fut) {
                       return fut.get();
                   });

    std::size_t first_index{0};
    std::size_t second_index{0};
    auto min_distance = UNREACHABLE;

    for(std::size_t i{0}; i < first.size(); i++) {
        for(std::size_t j{0}; j < second.size(); j++) {
            if(distances[i][j] < min_distance) {
                first_index = i;
                second_index = j;
                min_distance = distances[i][j];
            }
        }
    }

    return std::tuple{second[first_index],
                      second[second_index],
                      min_distance};
}

template<class MultiTargetPathFinder>
[[nodiscard]] auto checkSeparation(const pathfinding::PathQuerySystem<MultiTargetPathFinder>& multi_path_finder,
                                   const grid::GridCell& first,
                                   const grid::GridCell& second) noexcept
    -> std::optional<separation::Separation>
{
    using graph::UNREACHABLE;

    //find first center
    fmt::print("searching for cluster centers...\n");
    auto center_opt = findCenterCandidates(multi_path_finder, first, second);
    if(!center_opt) {
        return std::nullopt;
    }

    auto [first_center,
          second_center,
          center_to_center_distance] = center_opt.value();

	fmt::print("first center {}\n", first_center);
	fmt::print("second center {}\n", second_center);
    fmt::print("center to center distance: {}\n", center_to_center_distance);

    if(center_to_center_distance == UNREACHABLE) {
        return std::nullopt;
    }

    //calculate all distances from the clusters to its centers
    auto first_to_center_distances_fut = multi_path_finder.query(first_center, first);
    auto second_to_center_distances_fut = multi_path_finder.query(second_center, second);

    fmt::print("calculate distances from first cluster to its center...\n");
    auto first_to_center_distances = std::move(first_to_center_distances_fut.get());
    fmt::print("calculate distances from second cluster to its center...\n");
    auto second_to_center_distances = std::move(second_to_center_distances_fut.get());

    //check for all paths from center 'first' to center 'second'
    //if all paths go over the selected centers
    fmt::print("check if all the paths go over the cluster centers...\n");
    for(std::size_t i{0}; i < first.size(); i++) {
        auto from = first[i];
        auto distances_to_second = multi_path_finder.query(from, second).get();
        for(std::size_t j{0}; j < distances_to_second.size(); j++) {
            auto over_center_distance = first_to_center_distances[i] // distance from node i to first center
                + center_to_center_distance // distance from center to center
                + second_to_center_distances[j]; // distance from node j to second center;
            auto goes_over_center = distances_to_second[j] == over_center_distance;

            if(!goes_over_center) {

                return std::nullopt;
            }
        }
    }

    return Separation{first,
                      second,
                      first_center,
                      second_center,
                      center_to_center_distance};
}


} // namespace separation
