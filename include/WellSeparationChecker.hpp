#pragma once


#include <Distance.hpp>
#include <GridCell.hpp>
#include <GridGraph.hpp>
#include <PathQuerySystem.hpp>
#include <Separation.hpp>
#include <SimpleDijkstra.hpp>
#include <Utils.hpp>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <future>
#include <iterator>
#include <numeric>
#include <vector>

namespace separation {

//fast version have huge memory footprint because they internaly store the all-to-all distances
//from the gridcells to each other
template<class MultiTargetPathFinder>
[[nodiscard]] auto findCenterCandidatesFast(const pathfinding::PathQuerySystem<MultiTargetPathFinder>& multi_path_finder,
                                            const grid::GridCell& first,
                                            const grid::GridCell& second) noexcept
    -> std::optional<std::tuple<graph::Node,
                                graph::Node,
                                graph::Distance,
                                std::vector<std::vector<graph::Distance>>>>
{
    using graph::Distance;
    using graph::UNREACHABLE;

    auto distances = multi_path_finder.query(first, second).get();

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

    return std::tuple{first[first_index],
                      second[second_index],
                      min_distance,
                      distances};
}

template<class MultiTargetPathFinder>
[[nodiscard]] auto checkSeparationFast(const pathfinding::PathQuerySystem<MultiTargetPathFinder>& multi_path_finder,
                                       const grid::GridCell& first,
                                       const grid::GridCell& second) noexcept
    -> std::optional<separation::Separation>
{
    using graph::UNREACHABLE;

    if(first.isSuperSetOf(second) or first.isSubSetOf(second)) {
        return std::nullopt;
    }

    //find first center
    fmt::print("searching for cluster centers...\n");
    auto center_opt = findCenterCandidatesFast(multi_path_finder, first, second);
    if(!center_opt) {
        return std::nullopt;
    }

    auto [first_center,
          second_center,
          center_to_center_distance,
          all_to_all_distances] = std::move(center_opt.value());

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
        //all distances from i to the second cluster
        auto distances_to_second = all_to_all_distances[i];
        for(std::size_t j{0}; j < distances_to_second.size(); j++) {

            //check if the splitted path is unreachable
            //if so the total path also needs to be unreachable
            //otherwise return std::nullopt
            if(first_to_center_distances[i] == UNREACHABLE
               or second_to_center_distances[j] == UNREACHABLE
               or center_to_center_distance == UNREACHABLE) {

                //path over centers and shortest path are both
                // unreachable, this is ok
                if(distances_to_second[j] == UNREACHABLE) {
                    continue;
                }

                //otherwise nothing is ok and we return std::nullopt
                return std::nullopt;
            }

            auto over_center_distance = first_to_center_distances[i] // distance from node i to first center
                + center_to_center_distance // distance from center to center
                + second_to_center_distances[j]; // distance from node j to second center;

            if(distances_to_second[j] != over_center_distance) {
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


//slow versions recalculate a lot of distances but avoid a big memory footprint
template<class MultiTargetPathFinder>
[[nodiscard]] auto findCenterCandidatesSlow(const pathfinding::PathQuerySystem<MultiTargetPathFinder>& multi_path_finder,
                                            const grid::GridCell& first,
                                            const grid::GridCell& second) noexcept
    -> std::optional<std::tuple<graph::Node,
                                graph::Node,
                                graph::Distance>>
{
    using graph::Distance;
    using graph::UNREACHABLE;

    std::vector<std::future<std::pair<graph::Node, Distance>>> min_node_distance_futures;
    min_node_distance_futures.reserve(first.size());

    std::transform(std::cbegin(first),
                   std::cend(first),
                   std::back_inserter(min_node_distance_futures),
                   [&](const auto& node) {
                       return multi_path_finder
                           .queryAndThen(node,
                                         second,
                                         [&](const auto& distances) {
                                             std::size_t index{0};
                                             auto min_distance = UNREACHABLE;

                                             for(std::size_t i{0}; i < distances.size(); i++) {
                                                 if(distances[i] < min_distance) {
                                                     min_distance = distances[i];
                                                     index = i;
                                                 }
                                             }
                                             return std::pair{second[index], min_distance};
                                         });
                   });

    auto min_node_distances = util::collectFutures(std::move(min_node_distance_futures));

    std::size_t index{0};
    auto min_distance = UNREACHABLE;

    for(std::size_t i{0}; i < min_node_distances.size(); i++) {
        auto [_, distance] = min_node_distances[i];
        if(distance < min_distance) {
            min_distance = distance;
            index = i;
        }
    }

    return std::tuple{first[index],
                      min_node_distances[index].first,
                      min_distance};
}

template<class MultiTargetPathFinder>
[[nodiscard]] auto checkSeparationSlow(const pathfinding::PathQuerySystem<MultiTargetPathFinder>& multi_path_finder,
                                       const grid::GridCell& first,
                                       const grid::GridCell& second) noexcept
    -> std::optional<separation::Separation>
{
    using graph::UNREACHABLE;

    //find first center
    fmt::print("searching for cluster centers...\n");
    auto center_opt = findCenterCandidatesSlow(multi_path_finder, first, second);
    if(!center_opt) {
        return std::nullopt;
    }

    auto [first_center,
          second_center,
          center_to_center_distance] = std::move(center_opt.value());

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
        //all distances from i to the second cluster
        auto distances_to_second = multi_path_finder.query(first[i], second).get();
        for(std::size_t j{0}; j < distances_to_second.size(); j++) {

            //check if the splitted path is unreachable
            //if so the total path also needs to be unreachable
            //otherwise return std::nullopt
            if(first_to_center_distances[i] == UNREACHABLE
               or second_to_center_distances[j] == UNREACHABLE
               or center_to_center_distance == UNREACHABLE) {

                //path over centers and shortest path are both
                // unreachable, this is ok
                if(distances_to_second[j] == UNREACHABLE) {
                    continue;
                }

                //otherwise nothing is ok and we return std::nullopt
                return std::nullopt;
            }

            auto over_center_distance = first_to_center_distances[i] // distance from node i to first center
                + center_to_center_distance // distance from center to center
                + second_to_center_distances[j]; // distance from node j to second center;

            if(distances_to_second[j] != over_center_distance) {
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
