#include <array>
#include <graph/CellRowWalker.hpp>
#include <graph/GridCell.hpp>
#include <graph/GridCellIterator.hpp>
#include <graph/GridCorner.hpp>
#include <graph/Node.hpp>
#include <iostream>

using graph::CellRowWalker;
using graph::GridCorner;
using graph::CellRowIterator;
using graph::GridCell;


CellRowIterator::CellRowIterator(const GridCell& cell,
                                 difference_type idx)
    : cell_(cell),
      idx_(idx) {}

auto CellRowIterator::operator++() noexcept
    -> CellRowIterator&
{
    idx_++;
    return *this;
}

auto CellRowIterator::operator++(int) noexcept
    -> CellRowIterator
{
    auto ret = *this;
    ++(*this);
    return ret;
}

auto CellRowIterator::operator--() noexcept
    -> CellRowIterator&
{
    idx_--;
    return *this;
}

auto CellRowIterator::operator--(int) noexcept
    -> CellRowIterator
{
    auto ret = *this;
    --(*this);
    return ret;
}

auto CellRowIterator::operator+(difference_type rhs) const noexcept
    -> CellRowIterator
{
    return CellRowIterator{cell_, idx_ + rhs};
}

auto CellRowIterator::operator-(difference_type rhs) const noexcept
    -> CellRowIterator
{
    return CellRowIterator{cell_, idx_ - rhs};
}

auto CellRowIterator::operator+(const CellRowIterator& rhs) const noexcept
    -> difference_type
{
    return idx_ + rhs.idx_;
}

auto CellRowIterator::operator-(const CellRowIterator& rhs) const noexcept
    -> difference_type
{
    return idx_ - rhs.idx_;
}

auto CellRowIterator::operator+=(difference_type rhs) noexcept
    -> CellRowIterator&
{
    idx_ += rhs;
    return *this;
}

auto CellRowIterator::operator-=(difference_type rhs) noexcept
    -> CellRowIterator&
{
    idx_ -= rhs;
    return *this;
}
auto CellRowIterator::operator+=(const CellRowIterator& rhs) noexcept
    -> CellRowIterator&
{
    idx_ += rhs.idx_;
    return *this;
}

auto CellRowIterator::operator-=(const CellRowIterator& rhs) noexcept
    -> CellRowIterator&
{
    idx_ += rhs.idx_;
    return *this;
}

auto CellRowIterator::operator==(const CellRowIterator& other) const noexcept
    -> bool
{
    return idx_ == other.idx_ && cell_ == other.cell_;
}

auto CellRowIterator::operator!=(const CellRowIterator& other) const noexcept
    -> bool
{
    return !(*this == other);
}

auto CellRowIterator::operator<=(const CellRowIterator& other) const noexcept
    -> bool
{
    return *this < other or *this == other;
}

auto CellRowIterator::operator>=(const CellRowIterator& other) const noexcept
    -> bool
{
    return *this > other or *this == other;
}

auto CellRowIterator::operator<(const CellRowIterator& other) const noexcept
    -> bool
{
    return this->idx_ < other.idx_;
}
auto CellRowIterator::operator>(const CellRowIterator& other) const noexcept
    -> bool
{
    return this->idx_ > other.idx_;
}

auto CellRowIterator::operator*() noexcept -> GridCell
{
    return (*this)[idx_];
}

auto CellRowIterator::operator->() noexcept -> GridCell
{
    return **this;
}

auto CellRowIterator::operator[](int idx) noexcept -> GridCell
{
    auto top_left = GridCorner{cell_.getTopLeft().getRow() + idx,
                               cell_.getTopLeft().getColumn()};

    auto top_right = GridCorner{cell_.getTopRight().getRow() + idx,
                                cell_.getTopRight().getColumn()};

    auto bottom_left = GridCorner{cell_.getTopLeft().getRow() + idx + 1,
                                  cell_.getTopLeft().getColumn()};

    auto bottom_right = GridCorner{cell_.getTopRight().getRow() + idx + 1,
                                   cell_.getTopRight().getColumn()};

    return GridCell{top_left,
                    top_right,
                    bottom_left,
                    bottom_right};
}

CellRowWalker::CellRowWalker(const GridCell& cell)
    : cell_(cell) {}


auto CellRowWalker::begin() const noexcept
    -> CellRowIterator
{
    return CellRowIterator{cell_};
}

auto CellRowWalker::end() const noexcept
    -> CellRowIterator
{
    return CellRowIterator{cell_,
                           static_cast<long>(cell_.getHeight())};
}
