#pragma once

#include <Distance.hpp>
#include <GridCell.hpp>
#include <GridGraph.hpp>
#include <Path.hpp>

namespace separation {

class Separation
{
public:
    Separation(grid::GridCell first,
               grid::GridCell second,
               graph::Node first_center,
               graph::Node second_center,
               graph::Distance center_distance);
    Separation(const Separation&) = default;
    Separation(Separation&&) = default;

    auto operator=(const Separation&) noexcept
        -> Separation& = default;
    auto operator=(Separation&&) noexcept
        -> Separation& = default;

    [[nodiscard]] auto getCenterDistance() const noexcept
        -> graph::Distance;

    [[nodiscard]] auto getFirstCluster() const noexcept
        -> grid::GridCell;

    [[nodiscard]] auto getSecondCluster() const noexcept
        -> grid::GridCell;

    [[nodiscard]] auto getFirstClusterCenter() const noexcept
        -> graph::Node;

    [[nodiscard]] auto getSecondClusterCenter() const noexcept
        -> graph::Node;

    auto toFile(const graph::GridGraph& graph,
                std::string_view path) const noexcept
        -> void;

private:
    grid::GridCell first_;
    grid::GridCell second_;
    graph::Node first_center_;
    graph::Node second_center_;
    graph::Distance center_distance_;
};

} // namespace separation
