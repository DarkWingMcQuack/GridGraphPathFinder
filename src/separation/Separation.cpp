#include <fstream>
#include <pathfinding/Distance.hpp>
#include <pathfinding/Path.hpp>
#include <separation/Separation.hpp>

using separation::Separation;
using graph::Distance;
using graph::Node;
using graph::GridCell;

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


auto Separation::operator<(const Separation& other) const noexcept
    -> bool
{
    return first_.size() * second_.size()
        < other.first_.size() * other.second_.size();
}

auto Separation::getCenterDistance() const noexcept
    -> Distance
{
    return center_distance_;
}

auto Separation::getFirstCluster() const noexcept
    -> GridCell
{
    return first_;
}

auto Separation::getSecondCluster() const noexcept
    -> GridCell
{
    return second_;
}

auto Separation::getFirstClusterCenter() const noexcept
    -> Node
{
    return first_center_;
}

auto Separation::getSecondClusterCenter() const noexcept
    -> Node
{
    return second_center_;
}


auto Separation::toFile(std::string_view path) const noexcept
    -> void
{
    std::ofstream file{path.data()};
    for(auto node : first_) {
        file << "0: (" << node.row << ", " << node.column << ")\n";
    }
    for(auto node : second_) {
        file << "1: (" << node.row << ", " << node.column << ")\n";
    }

    file << "center: (" << first_center_.row << ", " << first_center_.column << ")\n";
    file << "center: (" << second_center_.row << ", " << second_center_.column << ")\n";
}
