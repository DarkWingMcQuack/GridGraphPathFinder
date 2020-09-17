#pragma once

#include <graph/GridCell.hpp>
#include <pathfinding/Distance.hpp>
#include <separation/Separation.hpp>
#include <separation/WellSeparationChecker.hpp>
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
    if(first == second and first.size() == 1) {
        return {};
    }

    const auto& graph = path_finder.getGraph();
    if(!graph.hasWalkableNode(first) or !graph.hasWalkableNode(second)) {
        return {};
    }

    auto separation_opt = checkSeparation(path_finder, first, second);

    if(separation_opt) {
        // fmt::print("found well separated pair with sizes {} and {}\n",
        //            graph.countNumberOfWalkableNodes(first),
        //            graph.countNumberOfWalkableNodes(second));
        return std::vector{std::move(separation_opt.value())};
    }

    //TODO: what to do with two nodes with no path between them?
    if(first.size() == second.size() and first.size() == 1) {
        return {
            ComplexSeparation{first,
                              second,
                              first[0],
                              second[0],
                              graph::UNREACHABLE}};
    }

    if(first.size() < second.size()) {
        return calculateSeparation(path_finder, second, first);
    }

    auto [split0, split1, split2, split3] = first.split();

    return util::concat(calculateSeparation(path_finder, split0, second),
                        calculateSeparation(path_finder, split1, second),
                        calculateSeparation(path_finder, split2, second),
                        calculateSeparation(path_finder, split3, second));
}

} // namespace impl

template<class PathFinder>
[[nodiscard]] auto calculateSeparation(PathFinder&& path_finder) noexcept
    -> std::vector<Separation>
{
    const auto& graph = path_finder.getGraph();
    auto root = graph.wrapGraphInCell();

    return impl::calculateSeparation(path_finder, root, root);
}

} // namespace separation
