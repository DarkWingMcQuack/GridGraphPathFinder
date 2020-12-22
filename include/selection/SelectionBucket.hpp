#pragma once

#include <graph/Node.hpp>
#include <selection/NodeSelection.hpp>
#include <vector>

namespace selection {

class SelectionBucket
{
public:
    SelectionBucket(std::vector<std::size_t> selections,
                    bool sort = true);

    [[nodiscard]] auto isSubSetOf(const SelectionBucket& other) const noexcept
        -> bool;

    [[nodiscard]] auto isSubSetOf(const std::vector<std::size_t>& other) const noexcept
        -> bool;

    [[nodiscard]] auto isSuperSetOf(const SelectionBucket& other) const noexcept
        -> bool;

    [[nodiscard]] auto isSuperSetOf(const std::vector<std::size_t>& other) const noexcept
        -> bool;

    [[nodiscard]] auto contains(const std::size_t& other) const noexcept
        -> bool;

    [[nodiscard]] auto size() const noexcept
        -> std::size_t;

    [[nodiscard]] auto getCommonSelection(const SelectionBucket& other) const noexcept
        -> std::optional<std::size_t>;

    [[nodiscard]] auto getSelections() const noexcept
        -> const std::vector<std::size_t>&;

    [[nodiscard]] auto getFirstIndex() const noexcept
        -> std::optional<std::size_t>;

    [[nodiscard]] auto getLastIndex() const noexcept
        -> std::optional<std::size_t>;

    [[nodiscard]] auto exclude(std::size_t selection) && noexcept
        -> SelectionBucket;

    [[nodiscard]] auto merge(SelectionBucket other) && noexcept
        -> SelectionBucket;

private:
    std::vector<std::size_t> selections_;
};

} // namespace selection
