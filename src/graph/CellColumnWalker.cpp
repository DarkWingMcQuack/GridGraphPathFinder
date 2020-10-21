#include <array>
#include <graph/CellColumnWalker.hpp>
#include <graph/GridCell.hpp>
#include <graph/GridCellIterator.hpp>
#include <graph/GridCorner.hpp>
#include <graph/Node.hpp>
#include <iostream>

using graph::CellColumnWalker;
using graph::GridCorner;
using graph::CellColumnIterator;
using graph::GridCell;


CellColumnIterator::CellColumnIterator(const GridCell& cell,
                                       difference_type idx)
    : cell_(cell),
      idx_(idx) {}

auto CellColumnIterator::operator++() noexcept
    -> CellColumnIterator&
{
    idx_++;
    return *this;
}

auto CellColumnIterator::operator++(int) noexcept
    -> CellColumnIterator
{
    auto ret = *this;
    ++(*this);
    return ret;
}

auto CellColumnIterator::operator--() noexcept
    -> CellColumnIterator&
{
    idx_--;
    return *this;
}

auto CellColumnIterator::operator--(int) noexcept
    -> CellColumnIterator
{
    auto ret = *this;
    --(*this);
    return ret;
}

auto CellColumnIterator::operator+(difference_type rhs) const noexcept
    -> CellColumnIterator
{
    return CellColumnIterator{cell_, idx_ + rhs};
}

auto CellColumnIterator::operator-(difference_type rhs) const noexcept
    -> CellColumnIterator
{
    return CellColumnIterator{cell_, idx_ - rhs};
}

auto CellColumnIterator::operator+(const CellColumnIterator& rhs) const noexcept
    -> difference_type
{
    return idx_ + rhs.idx_;
}

auto CellColumnIterator::operator-(const CellColumnIterator& rhs) const noexcept
    -> difference_type
{
    return idx_ - rhs.idx_;
}

auto CellColumnIterator::operator+=(difference_type rhs) noexcept
    -> CellColumnIterator&
{
    idx_ += rhs;
    return *this;
}

auto CellColumnIterator::operator-=(difference_type rhs) noexcept
    -> CellColumnIterator&
{
    idx_ -= rhs;
    return *this;
}
auto CellColumnIterator::operator+=(const CellColumnIterator& rhs) noexcept
    -> CellColumnIterator&
{
    idx_ += rhs.idx_;
    return *this;
}

auto CellColumnIterator::operator-=(const CellColumnIterator& rhs) noexcept
    -> CellColumnIterator&
{
    idx_ += rhs.idx_;
    return *this;
}

auto CellColumnIterator::operator==(const CellColumnIterator& other) const noexcept
    -> bool
{
    return idx_ == other.idx_ && cell_ == other.cell_;
}

auto CellColumnIterator::operator!=(const CellColumnIterator& other) const noexcept
    -> bool
{
    return !(*this == other);
}

auto CellColumnIterator::operator<=(const CellColumnIterator& other) const noexcept
    -> bool
{
    return *this < other or *this == other;
}

auto CellColumnIterator::operator>=(const CellColumnIterator& other) const noexcept
    -> bool
{
    return *this > other or *this == other;
}

auto CellColumnIterator::operator<(const CellColumnIterator& other) const noexcept
    -> bool
{
    return this->idx_ < other.idx_;
}
auto CellColumnIterator::operator>(const CellColumnIterator& other) const noexcept
    -> bool
{
    return this->idx_ > other.idx_;
}

auto CellColumnIterator::operator*() noexcept -> GridCell
{
    return (*this)[idx_];
}

auto CellColumnIterator::operator->() noexcept -> GridCell
{
    return **this;
}

auto CellColumnIterator::operator[](int idx) noexcept -> GridCell
{
    auto top_left = GridCorner{cell_.getTopLeft().getRow(),
                               cell_.getTopLeft().getColumn() +idx};

    auto bottom_right = GridCorner{cell_.getBottomRight().getRow(),
                                   cell_.getBottomRight().getColumn() + idx + 1};

    return GridCell{top_left,
                    bottom_right};
}

CellColumnWalker::CellColumnWalker(const GridCell& cell)
    : cell_(cell) {}


auto CellColumnWalker::begin() const noexcept
    -> CellColumnIterator
{
    return CellColumnIterator{cell_};
}

auto CellColumnWalker::end() const noexcept
    -> CellColumnIterator
{
    return CellColumnIterator{cell_,
                              static_cast<long>(cell_.getHeight())};
}
