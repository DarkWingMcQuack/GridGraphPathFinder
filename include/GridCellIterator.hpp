#pragma once

#include <Node.hpp>
#include <optional>

namespace grid {

//forward declaration to avoid cyclic header includes
class GridCell;

class GridCellIterator
{
public:
    // GridCellIterator traits
    using difference_type = long;
    using value_type = graph::Node;
    using pointer = graph::Node;
    using reference = graph::Node;
    using iterator_category = std::random_access_iterator_tag;

    GridCellIterator(const GridCell& cell,
                     difference_type idx = 0);

    auto operator++() noexcept
        -> GridCellIterator&;

    auto operator++(int) noexcept
        -> GridCellIterator;

    auto operator--() noexcept
        -> GridCellIterator&;

    auto operator--(int) noexcept
        -> GridCellIterator;

    auto operator+(difference_type rhs) const noexcept
        -> GridCellIterator;
    auto operator-(difference_type rhs) const noexcept
        -> GridCellIterator;

    auto operator+=(difference_type rhs) noexcept
        -> GridCellIterator&;
    auto operator-=(difference_type rhs) noexcept
        -> GridCellIterator&;

    auto operator==(const GridCellIterator& other) const noexcept
        -> bool;
    auto operator!=(const GridCellIterator& other) const noexcept
        -> bool;
    auto operator<=(const GridCellIterator& other) const noexcept
        -> bool;
    auto operator>=(const GridCellIterator& other) const noexcept
        -> bool;
    auto operator<(const GridCellIterator& other) const noexcept
        -> bool;
    auto operator>(const GridCellIterator& other) const noexcept
        -> bool;

    auto operator*() noexcept -> graph::Node;
    auto operator->() noexcept -> graph::Node;
    auto operator[](int) noexcept -> graph::Node;


private:
    const GridCell& cell_;
    difference_type idx_;
};

} // namespace grid
