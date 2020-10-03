#pragma once


#include <future>
#include <graph/GridCell.hpp>
#include <graph/GridGraph.hpp>
#include <iterator>
#include <numeric>
#include <pathfinding/Distance.hpp>
#include <separation/Separation.hpp>
#include <utils/Utils.hpp>
#include <vector>

namespace separation {

template<class PathFinder>
[[nodiscard]] auto findCenterCandidates(PathFinder& path_finder,
                                        const graph::GridCell& first,
                                        const graph::GridCell& second) noexcept
    -> std::tuple<graph::Node,
                  graph::Node,
                  graph::Distance,
                  bool>
{
    using graph::Distance;
    using graph::UNREACHABLE;

    auto min_distance = UNREACHABLE;
    std::optional<graph::Node> first_center;
    std::optional<graph::Node> second_center;

    auto is_trivial_separated = true;

    for(auto from : first) {
        for(auto to : second) {
            auto distance = path_finder.findDistance(from, to);
            auto trivial_distance = path_finder.findTrivialDistance(from, to);

            is_trivial_separated &= trivial_distance == distance;

            if(distance < min_distance) {
                min_distance = distance;
                first_center = from;
                second_center = to;
            }
        }
    }

    return std::tuple{first_center.value(),
                      second_center.value(),
                      min_distance,
                      is_trivial_separated};
}

template<class PathFinder>
[[nodiscard]] auto checkSeparation(PathFinder& path_finder,
                                   const graph::GridCell& first,
                                   const graph::GridCell& second) noexcept
    -> std::optional<separation::Separation>
{
    using graph::UNREACHABLE;
    using graph::Distance;

    if(first.isSubSetOf(second) or first.isSuperSetOf(second)) {
        return std::nullopt;
    }

    auto [first_center,
          second_center,
          center_to_center_distance,
          is_trivial_separation] = findCenterCandidates(path_finder, first, second);

    if(is_trivial_separation) {
        return TrivialSeparation(first, second);
    }

    //calculate all distances from the clusters to its centers
    std::vector<Distance> first_to_center_distances;
    std::transform(std::begin(first),
                   std::end(first),
                   std::back_inserter(first_to_center_distances),
                   [&path_finder, first_center = first_center](auto source) {
                       return path_finder.findDistance(first_center, source);
                   });

    std::vector<Distance> second_to_center_distances;
    std::transform(std::begin(second),
                   std::end(second),
                   std::back_inserter(second_to_center_distances),
                   [&path_finder, second_center = second_center](auto source) {
                       return path_finder.findDistance(second_center, source);
                   });


    //check for all paths from center 'first' to center 'second'
    //if all paths go over the selected centers
    for(std::size_t i{0}; i < first.size(); i++) {
        auto source = first[i];

        for(std::size_t j{0}; j < second.size(); j++) {
            auto target = second[j];

            auto optimal_distance = path_finder.findDistance(source, target);

            if(first_to_center_distances[i] == UNREACHABLE
               or second_to_center_distances[j] == UNREACHABLE
               or center_to_center_distance == UNREACHABLE) {

                //path over centers and shortest path are both
                // unreachable, this is ok
                if(optimal_distance == UNREACHABLE) {
                    continue;
                }

                //otherwise nothing is ok and we return std::nullopt
                return std::nullopt;
            }

            auto over_center_distance =
                first_to_center_distances[i] // distance from node i to first center
                + center_to_center_distance // distance from center to center
                + second_to_center_distances[j]; // distance from node j to second center;

            if(optimal_distance != over_center_distance) {
                return std::nullopt;
            }
        }
    }

    return ComplexSeparation{first,
                             second,
                             first_center,
                             second_center,
                             center_to_center_distance};
}


} // namespace separation
