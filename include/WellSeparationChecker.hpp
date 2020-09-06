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
                                        const grid::GridCell& from,
                                        const grid::GridCell& to) noexcept
    -> std::optional<graph::Node>
{
    using graph::Distance;
    using graph::UNREACHABLE;

    std::vector<std::future<Distance>> min_distances;
    std::transform(std::begin(from),
                   std::end(from),
                   std::back_inserter(min_distances),
                   [&](auto source) {
                       return multi_path_finder.queryAndThen(
                           source,
                           to,
                           [](const auto& distances) {
                               //find the minimum of all distances
                               return std::accumulate(std::cbegin(distances),
                                                      std::cend(distances),
                                                      UNREACHABLE,
                                                      [](auto acc, auto current) {
                                                          return std::min(acc, current);
                                                      });
                           });
                   });
    std::vector<Distance> collected_distances;
    collected_distances.reserve(min_distances.size());
    std::transform(std::make_move_iterator(std::begin(min_distances)),
                   std::make_move_iterator(std::end(min_distances)),
                   std::back_inserter(collected_distances),
                   [](auto fut) {
                       auto bla = fut.get();
                       return bla;
                   });

    auto min_iter = std::min_element(std::begin(collected_distances),
                                     std::end(collected_distances));

    if(min_iter == std::end(collected_distances)) {
        return std::nullopt;
    }

    auto min_idx = std::distance(std::begin(collected_distances),
                                 min_iter);

    return from[min_idx];
}

template<class MultiTargetPathFinder>
[[nodiscard]] auto checkSeparation(const pathfinding::PathQuerySystem<MultiTargetPathFinder>& multi_path_finder,
                                   const grid::GridCell& first,
                                   const grid::GridCell& second) noexcept
    -> std::optional<separation::Separation>
{
    using graph::UNREACHABLE;

    //find first center
    fmt::print("searching for first cluster center...\n");
    auto first_center_opt = findCenterCandidates(multi_path_finder, first, second);
    if(!first_center_opt) {
        return std::nullopt;
    }
    auto first_center = first_center_opt.value();
    fmt::print("found center {}\n", first_center);

    //find second center
    fmt::print("searching for second cluster center...\n");
    auto second_center_opt = findCenterCandidates(multi_path_finder, second, first);
    if(!second_center_opt) {
        return std::nullopt;
    }

    auto second_center = second_center_opt.value();
    fmt::print("found center {}\n", second_center);

    //calculate the center to center distance
    auto center_to_center_distance = multi_path_finder
                                         .query(first_center, second_center)
                                         .get();
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
