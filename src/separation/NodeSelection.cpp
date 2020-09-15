#include <fstream>
#include <graph/Node.hpp>
#include <separation/NodeSelection.hpp>

using separation::NodeSelection;


NodeSelection::NodeSelection(std::vector<graph::Node> left_selection,
                             std::vector<graph::Node> right_selection,
                             graph::Node center)
    : left_selection_(std::move(left_selection)),
      right_selection_(std::move(right_selection)),
      center_(center) {}

auto NodeSelection::getLeftSelection() const noexcept
    -> const std::vector<graph::Node>&
{
    return left_selection_;
}

auto NodeSelection::getRightSelection() const noexcept
    -> const std::vector<graph::Node>&
{
    return right_selection_;
}

auto NodeSelection::getCenter() const noexcept
    -> graph::Node
{
    return center_;
}

auto NodeSelection::toFile(std::string_view path) const noexcept
    -> void
{
    std::ofstream file{path.data()};
    for(auto node : left_selection_) {
        file << "0: (" << node.row << ", " << node.column << ")\n";
    }
    for(auto node : right_selection_) {
        file << "1: (" << node.row << ", " << node.column << ")\n";
    }

    file << "center: (" << center_.row << ", " << center_.column << ")\n";
}
