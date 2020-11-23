#pragma once

#include <graph/Node.hpp>
#include <selection/NodeSelection.hpp>
#include <vector>

namespace selection {

class SelectionBucket
{
    SelectionBucket(std::vector<NodeSelection> selections,
                    bool sort = true);

    [[nodiscard]] auto weight() const noexcept
        -> std::size_t;

    [[nodiscard]] auto isSubSetOf(const SelectionBucket& other) const noexcept
        -> bool;

    [[nodiscard]] auto isSubSetOf(const std::vector<NodeSelection>& other) const noexcept
        -> bool;

    [[nodiscard]] auto isSuperSetOf(const SelectionBucket& other) const noexcept
        -> bool;

    [[nodiscard]] auto isSuperSetOf(const std::vector<NodeSelection>& other) const noexcept
        -> bool;

    [[nodiscard]] auto contains(const NodeSelection& other) const noexcept
        -> bool;

    [[nodiscard]] auto canAnswer(const graph::Node& from,
                                 const graph::Node& to) const noexcept
        -> bool;

    [[nodiscard]] auto merge(const SelectionBucket& other) const noexcept
        -> SelectionBucket;

    [[nodiscard]] auto getSelections() const noexcept
        -> const std::vector<NodeSelection>&;

private:
    std::vector<NodeSelection> selections_;
};

} // namespace selection
