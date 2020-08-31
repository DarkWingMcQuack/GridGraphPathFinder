
#pragma once

#include <GridCell.hpp>
#include <functional>

namespace graph {

class GridGraph;

class GridGraphIterator
{
public:
    GridGraphIterator(const GridGraph& graph,
                      std::size_t cell_width,
                      std::size_t cell_height,
                      std::size_t top_left_possition = 0);

    auto operator++() noexcept
        -> GridGraphIterator&;

    auto operator++(int) noexcept
        -> GridGraphIterator;

    auto operator--() noexcept
        -> GridGraphIterator&;

    auto operator--(int) noexcept
        -> GridGraphIterator;

    auto operator==(GridGraphIterator other) const noexcept
        -> bool;
    auto operator!=(GridGraphIterator other) const noexcept
        -> bool;

    auto operator*() noexcept -> graph::Node;

    // GridGraphIterator traits
    using difference_type = std::size_t;
    using value_type = grid::GridCell;
    using pointer = const grid::GridCell*;
    using reference = const grid::GridCell&;
    using GridGraphIterator_category = std::bidirectional_iterator_tag;

private:
    const std::reference_wrapper<const GridGraph> graph_;
    const std::size_t cell_width_;
    const std::size_t cell_height_;
    std::size_t top_left_possition_;
};

} // namespace graph
