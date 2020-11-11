#pragma once

#include <graph/GridCell.hpp>
#include <pathfinding/Distance.hpp>
#include <pathfinding/Path.hpp>
#include <variant>

namespace graph {
class GridGraph;
}

namespace separation {

class ComplexSeparation
{
public:
    ComplexSeparation(graph::GridCell first,
                      graph::GridCell second,
                      graph::Node first_center,
                      graph::Node second_center,
                      graph::Distance center_distance);
    ComplexSeparation(const ComplexSeparation&) = default;
    ComplexSeparation(ComplexSeparation&&) = default;

    auto operator=(const ComplexSeparation&) noexcept
        -> ComplexSeparation& = default;
    auto operator=(ComplexSeparation&&) noexcept
        -> ComplexSeparation& = default;

    [[nodiscard]] auto getCenterDistance() const noexcept
        -> graph::Distance;

    [[nodiscard]] auto getFirstCluster() const noexcept
        -> graph::GridCell;

    [[nodiscard]] auto getSecondCluster() const noexcept
        -> graph::GridCell;

    [[nodiscard]] auto getFirstClusterCenter() const noexcept
        -> graph::Node;

    [[nodiscard]] auto getSecondClusterCenter() const noexcept
        -> graph::Node;

    [[nodiscard]] auto switchSides() const noexcept
        -> ComplexSeparation;

    auto toFile(std::string_view path) const noexcept
        -> void;

private:
    graph::GridCell first_;
    graph::GridCell second_;
    graph::Node first_center_;
    graph::Node second_center_;
    graph::Distance center_distance_;
};

class TrivialSeparation
{
public:
    TrivialSeparation(graph::GridCell first,
                      graph::GridCell second);


    [[nodiscard]] auto getFirstCluster() const noexcept
        -> graph::GridCell;

    [[nodiscard]] auto getSecondCluster() const noexcept
        -> graph::GridCell;

    [[nodiscard]] auto switchSides() const noexcept
        -> TrivialSeparation;

    auto toFile(std::string_view path) const noexcept
        -> void;

private:
    graph::GridCell first_;
    graph::GridCell second_;
};

using Separation = std::variant<ComplexSeparation,
                                TrivialSeparation>;

auto getFirstCluster(const Separation& sep) noexcept
    -> graph::GridCell;

auto getSecondCluster(const Separation& sep) noexcept
    -> graph::GridCell;

auto isSuperSetOf(const Separation& first, const Separation& second) noexcept
    -> bool;

auto isSubSetOf(const Separation& first, const Separation& second) noexcept
    -> bool;

auto canAnswer(const Separation& sep, graph::Node from, graph::Node to) noexcept
    -> bool;

auto weight(const Separation& sep) noexcept
    -> std::size_t;

auto toString(const Separation& sep) noexcept
    -> std::string;

auto switchSides(const Separation& sep) noexcept
    -> Separation;

auto smallestDistance(const Separation& sep,
                      const graph::GridGraph& graph) noexcept
    -> graph::Distance;

auto operator==(const Separation& lhs, const Separation& rhs) noexcept
    -> bool;

auto operator!=(const Separation& lhs, const Separation& rhs) noexcept
    -> bool;

auto operator<(const Separation& lhs, const Separation& rhs) noexcept
    -> bool;

auto toFile(const Separation& sep, std::string_view path) noexcept
    -> void;


auto sizeDistribution3DToFile(const std::vector<Separation>& separations,
                              std::string_view file_path) noexcept
    -> void;

auto sizeToDistanceToFile(const std::vector<Separation>& separations,
                          const graph::GridGraph& graph,
                          std::string_view file_path) noexcept
    -> void;


} // namespace separation
