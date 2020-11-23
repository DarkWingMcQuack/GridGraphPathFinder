#pragma once

#include <graph/GridCell.hpp>
#include <pathfinding/Distance.hpp>
#include <pathfinding/Path.hpp>
#include <variant>

namespace graph {
class GridGraph;
}

namespace separation {

class Separation
{
public:
    Separation(graph::GridCell first,
                      graph::GridCell second,
                      graph::Node first_center = graph::Node{0, 0},
                      graph::Node second_center = graph::Node{0, 0},
                      graph::Distance center_distance = -1);

    Separation(const Separation&) = default;
    Separation(Separation&&) = default;

    auto operator=(const Separation&) noexcept
        -> Separation& = default;
    auto operator=(Separation&&) noexcept
        -> Separation& = default;

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

    [[nodiscard]] auto isComplex() const noexcept
        -> bool;

    [[nodiscard]] auto isTrivial() const noexcept
        -> bool;

    [[nodiscard]] auto isSuperSetOf(const Separation& other) const noexcept
        -> bool;

    [[nodiscard]] auto isSubSetOf(const Separation& other) const noexcept
        -> bool;

    [[nodiscard]] auto canAnswer(graph::Node from, graph::Node to) const noexcept
        -> bool;

    [[nodiscard]] auto weight() const noexcept
        -> std::size_t;

    [[nodiscard]] auto switchSides() const noexcept
        -> Separation;

    [[nodiscard]] auto toString() const noexcept
        -> std::string;

    [[nodiscard]] auto smallestDistance(const graph::GridGraph& graph) const noexcept
        -> graph::Distance;

    auto toFile(std::string_view path) const noexcept
        -> void;

    auto toSmallFile(std::string_view path) const noexcept
        -> void;

private:
    graph::GridCell first_;
    graph::GridCell second_;
    graph::Node first_center_;
    graph::Node second_center_;
    graph::Distance center_distance_;
};

auto operator==(const Separation& lhs, const Separation& rhs) noexcept
    -> bool;

auto operator!=(const Separation& lhs, const Separation& rhs) noexcept
    -> bool;

auto operator<(const Separation& lhs, const Separation& rhs) noexcept
    -> bool;

auto fromFile(std::string_view path) noexcept
    -> std::optional<Separation>;

auto sizeDistribution3DToFile(const std::vector<Separation>& separations,
                              std::string_view file_path) noexcept
    -> void;

auto sizeToDistanceToFile(const std::vector<Separation>& separations,
                          const graph::GridGraph& graph,
                          std::string_view file_path) noexcept
    -> void;


} // namespace separation


