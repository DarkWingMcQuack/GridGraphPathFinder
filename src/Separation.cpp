#include <Distance.hpp>
#include <Path.hpp>
#include <Separation.hpp>

using separation::Separation;
using graph::Distance;
using graph::Node;
using grid::GridCell;

Separation::Separation(GridCell first,
                       GridCell second,
                       Node first_center,
                       Node second_center,
                       Distance center_distance)
    : first_(first),
      second_(second),
      first_center_(first_center),
      second_center_(second_center),
      center_distance_(center_distance) {}


auto Separation::getCenterDistance() const
    -> Distance
{
    return center_distance_;
}

auto Separation::getFirstCluster() const
    -> GridCell
{
    return first_;
}

auto Separation::getSecondCluster() const
    -> GridCell
{
    return second_;
}

auto Separation::getFirstClusterCenter() const
    -> Node
{
    return first_center_;
}

auto Separation::getSecondClusterCenter() const
    -> Node
{
    return second_center_;
}
