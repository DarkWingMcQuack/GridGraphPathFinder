#pragma once

#include <graph/GridCell.hpp>
#include <graph/GridGraph.hpp>
#include <pathfinding/Distance.hpp>
#include <pathfinding/Path.hpp>

namespace separation {

class Separation
{
public:
    Separation(graph::GridCell first,
               graph::GridCell second,
               graph::Node first_center,
               graph::Node second_center,
               graph::Distance center_distance);
    Separation(const Separation&) = default;
    Separation(Separation&&) = default;

    auto operator=(const Separation&) noexcept
        -> Separation& = default;
    auto operator=(Separation&&) noexcept
        -> Separation& = default;

    auto operator<(const Separation& other) const noexcept
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

} // namespace separation
