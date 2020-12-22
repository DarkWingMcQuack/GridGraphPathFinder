#pragma once

#include <graph/Node.hpp>
#include <vector>
#include <string_view>

namespace selection {

class NodeSelection
{
public:
    NodeSelection(std::vector<graph::Node> left_selection,
                  std::vector<graph::Node> right_selection,
                  graph::Node center);

    [[nodiscard]] auto getLeftSelection() const noexcept
        -> const std::vector<graph::Node>&;

    [[nodiscard]] auto getRightSelection() const noexcept
        -> const std::vector<graph::Node>&;

    [[nodiscard]] auto getLeftSelection() noexcept
        -> std::vector<graph::Node>&;

    [[nodiscard]] auto getRightSelection() noexcept
        -> std::vector<graph::Node>&;

    [[nodiscard]] auto canAnswer(graph::Node from, graph::Node to) const noexcept
        -> bool;

    [[nodiscard]] auto getCenter() const noexcept
        -> graph::Node;

    [[nodiscard]] auto isSubSetOf(const NodeSelection& other) const noexcept
        -> bool;

    [[nodiscard]] auto weight() const noexcept
        -> std::size_t;

    auto deleteFromLeft(const std::vector<graph::Node>& nodes) noexcept
        -> void;
    auto deleteFromRight(const std::vector<graph::Node>& nodes) noexcept
        -> void;

    auto toFile(std::string_view path) const noexcept
        -> void;

private:
    std::vector<graph::Node> left_selection_;
    std::vector<graph::Node> right_selection_;
    graph::Node center_;
};

} // namespace selection
