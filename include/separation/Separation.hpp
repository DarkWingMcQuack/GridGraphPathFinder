#pragma once

#include <graph/GridCell.hpp>
#include <graph/GridGraph.hpp>
#include <pathfinding/Distance.hpp>
#include <pathfinding/Path.hpp>
#include <variant>

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

    auto operator<(const ComplexSeparation& other) const noexcept
        -> bool;


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

auto toFile(const Separation& sep, std::string_view path) noexcept
    -> void;

} // namespace separation
