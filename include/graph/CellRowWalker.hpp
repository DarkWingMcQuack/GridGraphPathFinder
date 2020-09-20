#pragma once

#include <array>
#include <graph/GridCellIterator.hpp>
#include <graph/GridCorner.hpp>
#include <graph/Node.hpp>
#include <iostream>

namespace graph {

class CellRowIterator
{
public:
    using difference_type = long;
    using value_type = GridCell;
    using pointer = GridCell;
    using reference = GridCell;
    using iterator_category = std::random_access_iterator_tag;

    explicit CellRowIterator(const GridCell& cell,
                             difference_type idx = 0);

    auto operator++() noexcept
        -> CellRowIterator&;

    auto operator++(int) noexcept
        -> CellRowIterator;

    auto operator--() noexcept
        -> CellRowIterator&;

    auto operator--(int) noexcept
        -> CellRowIterator;

    auto operator+(difference_type rhs) const noexcept
        -> CellRowIterator;
    auto operator-(difference_type rhs) const noexcept
        -> CellRowIterator;

    auto operator+(const CellRowIterator& rhs) const noexcept
        -> difference_type;
    auto operator-(const CellRowIterator& rhs) const noexcept
        -> difference_type;

    auto operator+=(difference_type rhs) noexcept
        -> CellRowIterator&;
    auto operator-=(difference_type rhs) noexcept
        -> CellRowIterator&;
    auto operator+=(const CellRowIterator& rhs) noexcept
        -> CellRowIterator&;
    auto operator-=(const CellRowIterator& rhs) noexcept
        -> CellRowIterator&;

    auto operator==(const CellRowIterator& other) const noexcept
        -> bool;
    auto operator!=(const CellRowIterator& other) const noexcept
        -> bool;
    auto operator<=(const CellRowIterator& other) const noexcept
        -> bool;
    auto operator>=(const CellRowIterator& other) const noexcept
        -> bool;
    auto operator<(const CellRowIterator& other) const noexcept
        -> bool;
    auto operator>(const CellRowIterator& other) const noexcept
        -> bool;

    auto operator*() noexcept -> GridCell;
    auto operator->() noexcept -> GridCell;
    auto operator[](int) noexcept -> GridCell;


private:
    const GridCell& cell_;
    difference_type idx_;
};

class CellRowWalker
{
public:
    CellRowWalker(const GridCell& cell);


    [[nodiscard]] auto begin() const noexcept
        -> CellRowIterator;

    [[nodiscard]] auto end() const noexcept
        -> CellRowIterator;

private:
    const GridCell& cell_;
};

} // namespace graph
