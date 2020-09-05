#pragma once

#include <Node.hpp>
#include <optional>

namespace grid {

//forward declaration to avoid cyclic header includes
class GridCell;

class GridCellIterator
{
public:
    GridCellIterator(const GridCell& cell,
                     std::size_t idx = 0);

    auto operator++() noexcept
        -> GridCellIterator&;

    auto operator++(int) noexcept
        -> GridCellIterator;

    auto operator--() noexcept
        -> GridCellIterator&;

    auto operator--(int) noexcept
        -> GridCellIterator;

    auto operator==(GridCellIterator other) const noexcept
        -> bool;
    auto operator!=(GridCellIterator other) const noexcept
        -> bool;

    auto operator*() noexcept -> graph::Node;

    // GridCellIterator traits
    using difference_type = long;
    using value_type = graph::Node;
    using pointer = const graph::Node*;
    using reference = const graph::Node&;
    using iterator_category = std::bidirectional_iterator_tag;

private:
    const GridCell& cell_;
    std::size_t idx_;
};

} // namespace grid
