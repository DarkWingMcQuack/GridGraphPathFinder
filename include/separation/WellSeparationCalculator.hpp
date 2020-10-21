#pragma once

#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <graph/GridCell.hpp>
#include <pathfinding/Distance.hpp>
#include <separation/Separation.hpp>
#include <separation/WellSeparationChecker.hpp>
#include <unordered_set>
#include <utils/Utils.hpp>
#include <vector>

namespace separation {

namespace impl {

template<class PathFinder>
[[nodiscard]] auto calculateSeparation(PathFinder& path_finder,
                                       const graph::GridCell& first,
                                       const graph::GridCell& second) noexcept
    -> std::vector<Separation>
{
    //are first and second the same a grid with exactly one node
    if((first == second and first.size() == 1)) {
        return {};
    }

    //check if first and second have at least one walkable node
    const auto& graph = path_finder.getGraph();
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

    return utils::concat(calculateSeparation(path_finder, second, split0),
                         calculateSeparation(path_finder, second, split1),
                         calculateSeparation(path_finder, second, split2),
                         calculateSeparation(path_finder, second, split3));
}

} // namespace impl

namespace test {

[[nodiscard]] inline auto separationSanityCheck(const graph::GridGraph& graph,
                                                const std::vector<Separation>& separations) noexcept
    -> bool
{
    std::unordered_set<std::pair<graph::Node, graph::Node>> all_pairs;

    for(auto first : graph) {
        for(auto second : graph) {
            if(first == second) {
                continue;
            }
            all_pairs.emplace(first, second);
        }
    }

    for(const auto& sep : separations) {
        auto first = getFirstCluster(sep);
        auto second = getSecondCluster(sep);

        for(auto f : first) {
            for(auto s : second) {
                all_pairs.erase(std::pair{f, s});
                all_pairs.erase(std::pair{s, f});
            }
        }
    }

    fmt::print("{}\n", all_pairs.size());

    return all_pairs.empty();
}

} // namespace test

template<class PathFinder>
[[nodiscard]] auto calculateSeparation(const graph::GridGraph& graph) noexcept
    -> std::vector<Separation>
{
    auto root = graph.wrapGraphInCell();
    PathFinder path_finder{graph};

    return impl::calculateSeparation(path_finder, root, root);
}

} // namespace separation
