#include <GridCell.hpp>
#include <GridCellIterator.hpp>
#include <Node.hpp>
#include <cmath>

using grid::GridCell;
using grid::GridCellIterator;
using graph::Node;

GridCellIterator::GridCellIterator(const GridCell& cell,
                                   difference_type idx)
    : cell_(cell),
      idx_(idx) {}

auto GridCellIterator::operator++() noexcept
    -> GridCellIterator&
{
    idx_++;
    return *this;
}

auto GridCellIterator::operator++(int) noexcept
    -> GridCellIterator
{
    auto ret = *this;
    ++(*this);
    return ret;
}

auto GridCellIterator::operator--() noexcept
    -> GridCellIterator&
{
    idx_--;
    return *this;
}

auto GridCellIterator::operator--(int) noexcept
    -> GridCellIterator
{
    auto ret = *this;
    --(*this);
    return ret;
}

auto GridCellIterator::operator+(difference_type rhs) const noexcept
    -> GridCellIterator
{
    return GridCellIterator{cell_, idx_ + rhs};
}

auto GridCellIterator::operator-(difference_type rhs) const noexcept
    -> GridCellIterator
{
    return GridCellIterator{cell_, idx_ - rhs};
}

auto GridCellIterator::operator+(const GridCellIterator& rhs) const noexcept
    -> difference_type
{
    return idx_ + rhs.idx_;
}

auto GridCellIterator::operator-(const GridCellIterator& rhs) const noexcept
    -> difference_type 
{
    return idx_ - rhs.idx_;
}

auto GridCellIterator::operator+=(difference_type rhs) noexcept
    -> GridCellIterator&
{
    idx_ += rhs;
    return *this;
}

auto GridCellIterator::operator-=(difference_type rhs) noexcept
    -> GridCellIterator&
{
    idx_ -= rhs;
    return *this;
}

auto GridCellIterator::operator+=(const GridCellIterator& rhs) noexcept
    -> GridCellIterator&
{
    idx_ += rhs.idx_;
    return *this;
}

auto GridCellIterator::operator-=(const GridCellIterator& rhs) noexcept
    -> GridCellIterator&
{
    idx_ -= rhs.idx_;
    return *this;
}

auto GridCellIterator::operator==(const GridCellIterator& other) const noexcept
    -> bool
{
    return idx_ == other.idx_ && cell_ == other.cell_;
}

auto GridCellIterator::operator!=(const GridCellIterator& other) const noexcept
    -> bool
{
    return !(*this == other);
}


auto GridCellIterator::operator<=(const GridCellIterator& other) const noexcept
    -> bool
{
    return *this < other or *this == other;
}

auto GridCellIterator::operator>=(const GridCellIterator& other) const noexcept
    -> bool
{
    return *this > other or *this == other;
}

auto GridCellIterator::operator<(const GridCellIterator& other) const noexcept
    -> bool
{
    return this->idx_ < other.idx_;
}

auto GridCellIterator::operator>(const GridCellIterator& other) const noexcept
    -> bool
{
    return this->idx_ > other.idx_;
}

auto GridCellIterator::operator*() noexcept -> graph::Node
{
    return cell_[idx_];
}

auto GridCellIterator::operator->() noexcept -> graph::Node
{
    return cell_[idx_];
}

auto GridCellIterator::operator[](int idx) noexcept -> graph::Node
{
    return cell_[idx];
}
