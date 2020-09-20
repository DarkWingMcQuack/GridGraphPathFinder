#include <fstream>
#include <pathfinding/Distance.hpp>
#include <pathfinding/Path.hpp>
#include <separation/Separation.hpp>

using separation::ComplexSeparation;
using separation::TrivialSeparation;
using graph::Distance;
using graph::Node;
using graph::GridCell;

ComplexSeparation::ComplexSeparation(GridCell first,
                                     GridCell second,
                                     Node first_center,
                                     Node second_center,
                                     Distance center_distance)
    : first_(first),
      second_(second),
      first_center_(first_center),
      second_center_(second_center),
      center_distance_(center_distance) {}


auto ComplexSeparation::getCenterDistance() const noexcept
    -> Distance
{
    return center_distance_;
}

auto ComplexSeparation::getFirstCluster() const noexcept
    -> GridCell
{
    return first_;
}

auto ComplexSeparation::getSecondCluster() const noexcept
    -> GridCell
{
    return second_;
}

auto ComplexSeparation::getFirstClusterCenter() const noexcept
    -> Node
{
    return first_center_;
}

auto ComplexSeparation::getSecondClusterCenter() const noexcept
    -> Node
{
    return second_center_;
}


auto ComplexSeparation::toFile(std::string_view path) const noexcept
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


TrivialSeparation::TrivialSeparation(graph::GridCell first,
                                     graph::GridCell second)
    : first_(first),
      second_(second) {}

auto TrivialSeparation::getFirstCluster() const noexcept
    -> graph::GridCell
{
    return first_;
}

auto TrivialSeparation::getSecondCluster() const noexcept
    -> graph::GridCell
{
    return second_;
}

auto TrivialSeparation::toFile(std::string_view path) const noexcept
    -> void
{
    std::ofstream file{path.data()};
    for(auto node : first_) {
        file << "0: (" << node.row << ", " << node.column << ")\n";
    }
    for(auto node : second_) {
        file << "1: (" << node.row << ", " << node.column << ")\n";
    }
}


auto separation::getFirstCluster(const Separation& sep) noexcept
    -> graph::GridCell
{
    return std::visit(
        [](const auto& separation) {
            return separation.getFirstCluster();
        },
        sep);
}

auto separation::getSecondCluster(const Separation& sep) noexcept
    -> graph::GridCell
{
    return std::visit(
        [](const auto& separation) {
            return separation.getSecondCluster();
        },
        sep);
}


auto separation::operator==(const Separation& lhs, const Separation& rhs) noexcept
    -> bool
{
    return (getFirstCluster(lhs) == getFirstCluster(rhs)
            and getSecondCluster(lhs) == getSecondCluster(rhs))
        or (getFirstCluster(lhs) == getSecondCluster(rhs)
            and getSecondCluster(lhs) == getFirstCluster(rhs));
}

auto separation::operator!=(const Separation& lhs, const Separation& rhs) noexcept
    -> bool
{
    return !(lhs == rhs);
}


auto separation::toFile(const Separation& sep, std::string_view path) noexcept
    -> void

{
    std::visit(
        [=](const auto& separation) {
            return separation.toFile(path);
        },
        sep);
}
