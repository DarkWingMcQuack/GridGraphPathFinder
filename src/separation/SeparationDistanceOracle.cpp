#include <fmt/ostream.h>
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
        auto left = sep.getFirstCluster();
        for(auto n : left) {
            auto idx = getIndex(n);
            separation_lookup_[idx].emplace_back(sep);
        }

        auto right = sep.getSecondCluster();
        auto switched_sep = sep.switchSides();
        for(auto n : right) {
            auto idx = getIndex(n);
            separation_lookup_[idx].emplace_back(switched_sep);
        }
    }

    for(auto& vec : separation_lookup_) {
        std::sort(std::begin(vec),
                  std::end(vec),
                  [](const auto& lhs, const auto& rhs) {
                      return lhs.getSecondCluster() < rhs.getSecondCluster();
                  });
    }
}

auto SeparationDistanceOracle::findDistance(graph::Node from, graph::Node to) const noexcept
    -> graph::Distance
{
    if(from == to) {
        return 0;
    }

    if(graph_.areNeighbours(from, to)) {
        return 1;
    }

    const auto from_idx = getIndex(from);

    const auto& from_separations = separation_lookup_[from_idx];

    auto separation = findSeparationContaining(from_separations, to);

    if(separation.isComplex()) {
        auto left_center = separation.getFirstClusterCenter();
        auto right_center = separation.getSecondClusterCenter();
        auto center_distance = separation.getCenterDistance();

        if(left_center == from and right_center == to) {
            return separation.getCenterDistance();
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

namespace {

auto PrivateBinarySearch(const std::vector<Separation>& vec, int l, int r, int index, Node value) -> Separation
{
    if(vec[index].getSecondCluster().isInCell(value)) {
        return vec[index];
    }

    if(vec[l].getSecondCluster().isInCell(value)) {
        return vec[l];
    }

    if(vec[r].getSecondCluster().isInCell(value)) {
        return vec[r];
    }

    fmt::print("node z: {}\n", value.zScore());
    fmt::print("grid z: {}\n", vec[index].getSecondCluster().getTopLeft().zScore());
    fmt::print("(2)l: {}, r:{}, idx:{}, node:{}, grid:{}\n", l, r, index, value, vec[index].getSecondCluster());

    auto wrapped = graph::GridCell::wrapInCell(value);

    if(vec[index].getSecondCluster().getZScoreRange().first < wrapped.getZScoreRange().first) {
        return PrivateBinarySearch(vec, index + 1, r, (index + 1 + r) / 2, value);
    }

    return PrivateBinarySearch(vec, l, index - 1, (index - 1 + l) / 2, value);
}


auto BinarySearch(const std::vector<Separation>& vec, int l, int r, Node value) -> Separation
{
    for(int i = 0; i < vec.size(); i++) {
        if(vec[i].getSecondCluster().isInCell(value)) {
            fmt::print("searched cluster is: {}, at index: {}\n", vec[i].getSecondCluster(), i);
        }
    }
    return PrivateBinarySearch(vec, l, r, (l + r) / 2, value);
}

} // namespace

auto SeparationDistanceOracle::findSeparationContaining(const std::vector<Separation>& separations,
                                                        const graph::Node& n) const noexcept
    -> Separation
{
    return *std::find_if(std::cbegin(separations),
                         std::cend(separations),
                         [&](auto separation) {
                             auto second = separation.getSecondCluster();
                             return second.isInCell(n);
                         });
}

auto SeparationDistanceOracle::getIndex(graph::Node n) const noexcept
    -> std::size_t
{
    return n.row * graph_.getWidth() + n.column;
}
