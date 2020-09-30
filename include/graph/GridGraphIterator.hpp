
#pragma once

#include <functional>
#include <graph/Node.hpp>

namespace graph {

class GridGraph;

class GridGraphIterator
{
public:
    // GridGraphIterator traits
    using difference_type = std::size_t;
    using value_type = graph::Node;
    using pointer = graph::Node;
    using reference = graph::Node;
    using iterator_category = std::forward_iterator_tag;

    GridGraphIterator(const GridGraph& graph,
                      std::size_t idx = 0);

    auto operator++() noexcept
        -> GridGraphIterator&;
    auto operator++(int) noexcept
        -> GridGraphIterator;

    auto operator+=(difference_type rhs) noexcept
        -> GridGraphIterator&;
    auto operator+=(const GridGraphIterator& rhs) noexcept
        -> GridGraphIterator&;

    auto operator==(const GridGraphIterator& other) const noexcept
        -> bool;
    auto operator!=(const GridGraphIterator& other) const noexcept
        -> bool;
    auto operator<=(const GridGraphIterator& other) const noexcept
        -> bool;
    auto operator>=(const GridGraphIterator& other) const noexcept
        -> bool;
    auto operator<(const GridGraphIterator& other) const noexcept
        -> bool;
    auto operator>(const GridGraphIterator& other) const noexcept
        -> bool;

    auto operator*() const noexcept -> graph::Node;
    auto operator->() const noexcept -> graph::Node;

private:
    auto getNodeAtIdx(std::size_t idx) const noexcept -> graph::Node;

private:
    const std::reference_wrapper<const GridGraph> graph_;
    std::size_t idx_;
    const std::size_t max_idx_;
};

} // namespace graph
