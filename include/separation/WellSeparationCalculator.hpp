#pragma once

#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <graph/GridCell.hpp>
#include <pathfinding/Distance.hpp>
#include <separation/Separation.hpp>
#include <separation/WellSeparationChecker.hpp>
#include <set>
#include <unordered_set>
#include <utils/Utils.hpp>
#include <vector>

namespace separation {

namespace impl {

template<class PathFinder>
[[nodiscard]] auto calculateSeparation(PathFinder& path_finder,
                                       graph::GridCell first,
                                       graph::GridCell second,
                                       std::unordered_set<std::pair<graph::GridCell, graph::GridCell>>& already_visited) noexcept
    -> std::vector<Separation>
{
    auto was_already_visited =
        already_visited.find(std::pair{first, second})
            != std::end(already_visited)
        or already_visited.find(std::pair{second, first})
            != std::end(already_visited);

    if(was_already_visited) {
        return {};
    }


    already_visited.emplace(first, second);

    if(first.size() < second.size()) {
        std::swap(first, second);
    }

    //are first and second the same a grid with exactly one node
    if((first == second and first.size() == 1)) {
        return {};
    }

    const auto& graph = path_finder.getGraph();

    //check if first and second have at least one walkable node
    if(!graph.hasWalkableNode(first) or !graph.hasWalkableNode(second)) {
        return {};
    }

    //if two clusters only have one node and they are neighbors, then ignore them
    if(first.size() == 1
       and second.size() == 1
       and path_finder.getGraph().areNeighbours(first[0], second[0])) {
        return {};
    }

    if(auto separation_opt = checkSeparation(path_finder, first, second)) {
        return std::vector{std::move(separation_opt.value())};
    }


    auto [split0, split1, split2, split3] = first.split();

    return utils::concat(calculateSeparation(path_finder, second, split0, already_visited),
                         calculateSeparation(path_finder, second, split1, already_visited),
                         calculateSeparation(path_finder, second, split2, already_visited),
                         calculateSeparation(path_finder, second, split3, already_visited));
}

} // namespace impl

namespace test {

[[nodiscard]] inline auto separationSanityCheck(const graph::GridGraph& graph,
                                                const std::vector<Separation>& separations) noexcept
    -> bool
{
    for(auto first : graph) {
        for(auto second : graph) {
            if(first == second or graph.areNeighbours(first, second)) {
                continue;
            }

            auto valid = std::any_of(std::begin(separations),
                                     std::end(separations),
                                     [&](auto sep) {
                                         return canAnswer(sep, first, second);
                                     });

            if(!valid) {
                return false;
            }
        }
    }

    return true;
}

} // namespace test

template<class PathFinder>
[[nodiscard]] auto calculateSeparation(const graph::GridGraph& graph) noexcept
    -> std::vector<Separation>
{
    auto root = graph.wrapGraphInCell();
    PathFinder path_finder{graph};

    std::unordered_set<std::pair<graph::GridCell, graph::GridCell>> visited;
    return impl::calculateSeparation(path_finder, root, root, visited);
}

} // namespace separation
