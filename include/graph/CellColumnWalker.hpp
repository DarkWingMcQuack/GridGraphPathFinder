#pragma once

#include <array>
#include <graph/GridCellIterator.hpp>
#include <graph/GridCorner.hpp>
#include <graph/Node.hpp>
#include <iostream>

namespace graph {

class CellColumnIterator
{
public:
    using difference_type = long;
    using value_type = GridCell;
    using pointer = GridCell;
    using reference = GridCell;
    using iterator_category = std::random_access_iterator_tag;

    explicit CellColumnIterator(const GridCell& cell,
                                difference_type idx = 0);

    auto operator++() noexcept
        -> CellColumnIterator&;

    auto operator++(int) noexcept
        -> CellColumnIterator;

    auto operator--() noexcept
        -> CellColumnIterator&;

    auto operator--(int) noexcept
        -> CellColumnIterator;

    auto operator+(difference_type rhs) const noexcept
        -> CellColumnIterator;
    auto operator-(difference_type rhs) const noexcept
        -> CellColumnIterator;

    auto operator+(const CellColumnIterator& rhs) const noexcept
        -> difference_type;
    auto operator-(const CellColumnIterator& rhs) const noexcept
        -> difference_type;

    auto operator+=(difference_type rhs) noexcept
        -> CellColumnIterator&;
    auto operator-=(difference_type rhs) noexcept
        -> CellColumnIterator&;
    auto operator+=(const CellColumnIterator& rhs) noexcept
        -> CellColumnIterator&;
    auto operator-=(const CellColumnIterator& rhs) noexcept
        -> CellColumnIterator&;

    auto operator==(const CellColumnIterator& other) const noexcept
        -> bool;
    auto operator!=(const CellColumnIterator& other) const noexcept
        -> bool;
    auto operator<=(const CellColumnIterator& other) const noexcept
        -> bool;
    auto operator>=(const CellColumnIterator& other) const noexcept
        -> bool;
    auto operator<(const CellColumnIterator& other) const noexcept
        -> bool;
    auto operator>(const CellColumnIterator& other) const noexcept
        -> bool;

    auto operator*() noexcept -> GridCell;
    auto operator->() noexcept -> GridCell;
    auto operator[](int) noexcept -> GridCell;


private:
    const GridCell& cell_;
    difference_type idx_;
};

class CellColumnWalker
{
public:
    CellColumnWalker(const GridCell& cell);


    [[nodiscard]] auto begin() const noexcept
        -> CellColumnIterator;

    [[nodiscard]] auto end() const noexcept
        -> CellColumnIterator;

private:
    const GridCell& cell_;
};

} // namespace graph
