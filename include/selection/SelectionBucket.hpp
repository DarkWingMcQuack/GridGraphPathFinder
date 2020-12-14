#pragma once

#include <graph/Node.hpp>
#include <selection/NodeSelection.hpp>
#include <vector>

namespace selection {

class SelectionBucket
{
public:
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

    [[nodiscard]] auto isSuperSetOf(const utils::RefVec<NodeSelection>& other) const noexcept
        -> bool;

    [[nodiscard]] auto contains(const NodeSelection& other) const noexcept
        -> bool;

    [[nodiscard]] auto canAnswer(const graph::Node& from,
                                 const graph::Node& to) const noexcept
        -> bool;

    //the result referenced lives as long as the given object other
    [[nodiscard]] auto getCommonSelection(const SelectionBucket& other) const noexcept
        -> std::optional<std::reference_wrapper<const NodeSelection>>;

    [[nodiscard]] auto getSelections() const noexcept
        -> const std::vector<NodeSelection>&;

    [[nodiscard]] auto getFirstIndex() const noexcept
        -> std::optional<std::size_t>;

    [[nodiscard]] auto getLastIndex() const noexcept
        -> std::optional<std::size_t>;

    [[nodiscard]] auto exclude(const NodeSelection& selection) && noexcept
        -> SelectionBucket;

    [[nodiscard]] auto merge(SelectionBucket other) && noexcept
        -> SelectionBucket;

private:
    std::vector<NodeSelection> selections_;
};

} // namespace selection
