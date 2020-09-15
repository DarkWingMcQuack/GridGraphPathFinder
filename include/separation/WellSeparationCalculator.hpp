#pragma once


#include <fmt/core.h>
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
            Separation{first,
                       second,
                       first[0],
                       second[0],
                       graph::UNREACHABLE}};
    }

    if(first.size() < second.size()) {
        return calculateSeparation(path_finder, second, first);
    }

    auto [split0, split1, split2, split3] = first.split();

    // fmt::print("width: {}\n", split0.getWidth());
    // fmt::print("width: {}\n", split1.getWidth());
    // fmt::print("width: {}\n", split2.getWidth());
    // fmt::print("width: {}\n", split3.getWidth());

    // fmt::print("height: {}\n", split0.getHeight());
    // fmt::print("height: {}\n", split1.getHeight());
    // fmt::print("height: {}\n", split2.getHeight());
    // fmt::print("height: {}\n", split3.getHeight());

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
    fmt::print("wasdad\n");
    auto root = graph.wrapGraphInCell();

    return impl::calculateSeparation(path_finder, root, root);
}

} // namespace separation
