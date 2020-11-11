#pragma once

#include <graph/GridGraph.hpp>
#include <separation/Separation.hpp>
#include <vector>

namespace separation {

class SeparationDistanceOracle
{
public:
    SeparationDistanceOracle(const graph::GridGraph& graph,
                             const std::vector<Separation>& separations) noexcept;

    [[nodiscard]] auto findDistance(graph::Node from, graph::Node to) const noexcept
        -> graph::Distance;

private:
    [[nodiscard]] auto getIndex(graph::Node n) const noexcept
        -> std::size_t;

private:
    const graph::GridGraph& graph_;
    std::vector<std::vector<Separation>> separation_lookup_;
};

} // namespace separation
