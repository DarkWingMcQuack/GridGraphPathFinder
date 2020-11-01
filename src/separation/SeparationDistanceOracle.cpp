#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <separation/Separation.hpp>
#include <separation/SeparationDistanceOracle.hpp>
#include <vector>

using separation::Separation;
using separation::SeparationDistanceOracle;
using graph::Node;
using graph::GridGraph;

SeparationDistanceOracle::SeparationDistanceOracle(const graph::GridGraph& graph,
                                                   const std::vector<Separation>& separations) noexcept
    : graph_(graph),
      separation_lookup_(graph.getWidth() * graph.getHeight(),
						 std::vector<Separation>{})

{
    for(auto sep : separations) {
        auto left = getFirstCluster(sep);

        for(auto n : left) {
            auto idx = getIndex(n);
            separation_lookup_[idx].emplace_back(sep);
        }

        auto right = getSecondCluster(sep);
        sep = switchSides(sep);
        for(auto n : right) {
            auto idx = getIndex(n);
            separation_lookup_[idx].emplace_back(sep);
        }
    }
}

[[nodiscard]] auto SeparationDistanceOracle::findDistance(graph::Node from, graph::Node to) const noexcept
    -> graph::Distance
{
    if(graph_.areNeighbours(from, to)) {
        return 1;
    }

    auto from_idx = getIndex(from);
    const auto& from_separations = separation_lookup_[from_idx];

    auto separation = *std::find_if(std::cbegin(from_separations),
                                    std::end(from_separations),
                                    [&](auto sep) {
                                        auto right = getSecondCluster(sep);
                                        return right.isInCell(to);
                                    });

    if(std::holds_alternative<ComplexSeparation>(separation)) {
        const auto& sep = std::get<ComplexSeparation>(separation);
        auto left_center = sep.getFirstClusterCenter();
        auto right_center = sep.getSecondClusterCenter();
        auto center_distance = sep.getCenterDistance();

        if(left_center == from and right_center == to) {
            return sep.getCenterDistance();
        }

        if(left_center == from) {
            return center_distance + findDistance(right_center, to);
        }

        if(right_center == to) {
            return center_distance + findDistance(from, left_center);
        }

        return center_distance
            + findDistance(from, left_center)
            + findDistance(right_center, to);
    }

    //if it is not complex separated, then it is trivial separated and
    //we can simply return the trivial distance
    return graph_.getTrivialDistance(from, to);
}

auto SeparationDistanceOracle::getIndex(graph::Node n) const noexcept
    -> std::size_t
{
    return n.row * graph_.getWidth() + n.column;
}
