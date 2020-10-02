#pragma once

#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <queue>
#include <vector>

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

    [[nodiscard]] auto getCenter() const noexcept
        -> graph::Node;

    auto toFile(std::string_view path) const noexcept
        -> void;

private:
    std::vector<graph::Node> left_selection_;
    std::vector<graph::Node> right_selection_;
    graph::Node center_;
};

} // namespace selction
