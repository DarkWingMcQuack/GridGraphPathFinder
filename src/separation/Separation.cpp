#include <fstream>
#include <map>
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

auto separation::isSuperSetOf(const Separation& first, const Separation& second) noexcept
    -> bool
{
    auto first_left = getFirstCluster(first);
    auto first_right = getSecondCluster(first);

    auto second_left = getFirstCluster(second);
    auto second_right = getSecondCluster(second);

    return (first_left.isSuperSetOf(second_left) and first_right.isSuperSetOf(second_right))
        or (first_left.isSuperSetOf(second_right) and first_right.isSuperSetOf(second_left));
}

auto separation::isSubSetOf(const Separation& first, const Separation& second) noexcept
    -> bool
{
    auto first_left = getFirstCluster(first);
    auto first_right = getSecondCluster(first);

    auto second_left = getFirstCluster(second);
    auto second_right = getSecondCluster(second);

    return (first_left.isSubSetOf(second_left) and first_right.isSubSetOf(second_right))
        or (first_left.isSubSetOf(second_right) and first_right.isSubSetOf(second_left));
}

auto separation::canAnswer(const Separation& sep, graph::Node from, graph::Node to) noexcept
    -> bool
{
    auto left = getFirstCluster(sep);
    auto right = getSecondCluster(sep);

    auto left_from_iter = std::find(std::begin(left), std::end(left), from);
    auto left_to_iter = std::find(std::begin(left), std::end(left), to);

    auto right_from_iter = std::find(std::begin(right), std::end(right), from);
    auto right_to_iter = std::find(std::begin(right), std::end(right), to);

    return (left_from_iter != std::end(left) and right_to_iter != std::end(right))
        or (right_from_iter != std::end(right) and left_to_iter != std::end(left));
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


auto separation::operator<(const Separation& lhs, const Separation& rhs) noexcept
    -> bool
{
    return weight(lhs) < weight(rhs);
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


auto separation::weight(const Separation& sep) noexcept
    -> std::size_t
{
    return getFirstCluster(sep).size()
        * getSecondCluster(sep).size();
}

auto separation::toString(const Separation& sep) noexcept
    -> std::string
{
    std::stringstream ss;
    ss << getFirstCluster(sep)
       << ":"
       << getSecondCluster(sep)
       << "("
       << weight(sep)
       << ")";

    return ss.str();
}


auto separation::sizeDistribution3DToFile(const std::vector<Separation>& separations,
                                          std::string_view file_path) noexcept
    -> void
{
    std::map<std::pair<std::int64_t, std::int64_t>, std::int64_t> sep_distribution;

    for(const auto& sep : separations) {
        auto first = getFirstCluster(sep).size();
        auto second = getSecondCluster(sep).size();

        auto smaller = std::min(first, second);
        auto larger = std::max(first, second);

        sep_distribution[std::pair{smaller, larger}]++;
    }

    std::ofstream file{file_path.data()};
    for(auto [pair, amount] : sep_distribution) {
        auto [smaller, larger] = pair;
        file << smaller << ", " << larger << ", " << amount << "\n";
    }
}
