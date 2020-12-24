#include <algorithm>
#include <fstream>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <selection/NodeSelection.hpp>

using selection::NodeSelection;
using graph::Node;


NodeSelection::NodeSelection(std::vector<graph::Node> left_selection,
                             std::vector<graph::Node> right_selection,
                             graph::Node center)
    : left_selection_(std::move(left_selection)),
      right_selection_(std::move(right_selection)),
      center_(center)
{
    std::sort(std::begin(left_selection_),
              std::end(left_selection_));

    std::sort(std::begin(right_selection_),
              std::end(right_selection_));
}


auto NodeSelection::weight() const noexcept
    -> std::size_t
{
    return left_selection_.size() * right_selection_.size();
}

auto NodeSelection::getLeftSelection() const noexcept
    -> const std::vector<Node>&
{
    return left_selection_;
}

auto NodeSelection::getRightSelection() const noexcept
    -> const std::vector<Node>&
{
    return right_selection_;
}
auto NodeSelection::getLeftSelection() noexcept
    -> std::vector<Node>&
{
    return left_selection_;
}

auto NodeSelection::getRightSelection() noexcept
    -> std::vector<Node>&
{
    return right_selection_;
}

auto NodeSelection::getCenter() const noexcept
    -> graph::Node
{
    return center_;
}

auto NodeSelection::deleteFromLeft(const std::vector<graph::Node>& nodes) noexcept
    -> void
{
    for(auto n : nodes) {
        auto iter = std::lower_bound(std::begin(left_selection_),
                                     std::end(left_selection_),
                                     n);
        if(iter != std::end(left_selection_) and n == *iter) {
            left_selection_.erase(iter);
        }
    }
}
auto NodeSelection::deleteFromRight(const std::vector<graph::Node>& nodes) noexcept
    -> void
{
    for(auto n : nodes) {
        auto iter = std::lower_bound(std::begin(right_selection_),
                                     std::end(right_selection_),
                                     n);
        if(iter != std::end(right_selection_) and n == *iter) {
            right_selection_.erase(iter);
        }
    }
}


auto NodeSelection::isSubSetOf(const NodeSelection& other) const noexcept
    -> bool
{
    return (std::includes(std::begin(other.left_selection_),
                          std::end(other.left_selection_),
                          std::begin(left_selection_),
                          std::end(left_selection_))
            and std::includes(std::begin(other.right_selection_),
                              std::end(other.right_selection_),
                              std::begin(right_selection_),
                              std::end(right_selection_)))
        or (std::includes(std::begin(other.right_selection_),
                          std::end(other.right_selection_),
                          std::begin(left_selection_),
                          std::end(left_selection_))
            and std::includes(std::begin(other.left_selection_),
                              std::end(other.left_selection_),
                              std::begin(right_selection_),
                              std::end(right_selection_)));
}

auto NodeSelection::canAnswer(Node from, Node to) const noexcept
    -> bool
{
    return (std::binary_search(std::begin(left_selection_),
                               std::end(left_selection_),
                               from)
            and std::binary_search(std::begin(right_selection_),
                                   std::end(right_selection_),
                                   to))

        or (std::binary_search(std::begin(left_selection_),
                               std::end(left_selection_),
                               to)
            and std::binary_search(std::begin(right_selection_),
                                   std::end(right_selection_),
                                   from));
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
