#pragma once

#include <Distance.hpp>
#include <GridCell.hpp>
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

    auto operator=(const Separation&) noexcept -> Separation& = default;
    auto operator=(Separation&&) noexcept -> Separation& = default;

    auto getCenterDistance() const
        -> graph::Distance;

    auto getFirstCluster() const
        -> grid::GridCell;

    auto getSecondCluster() const
        -> grid::GridCell;

    auto getFirstClusterCenter() const
        -> graph::Node;

    auto getSecondClusterCenter() const
        -> graph::Node;

private:
    grid::GridCell first_;
    grid::GridCell second_;
    graph::Node first_center_;
    graph::Node second_center_;
    graph::Distance center_distance_;
};

} // namespace separation
