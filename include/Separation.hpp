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
    const grid::GridCell first_;
    const grid::GridCell second_;
    const graph::Node first_center_;
    const graph::Node second_center_;
    const graph::Distance center_distance_;
};

} // namespace separation
