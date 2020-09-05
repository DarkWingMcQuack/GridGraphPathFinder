#include <GridCell.hpp>
#include <GridCellIterator.hpp>
#include <Node.hpp>
#include <cmath>

using grid::GridCell;
using grid::GridCellIterator;
using graph::Node;

GridCellIterator::GridCellIterator(const GridCell& cell,
                                   std::size_t idx)
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

auto GridCellIterator::operator==(GridCellIterator other) const noexcept
    -> bool
{
    return idx_ == other.idx_ && cell_ == other.cell_;
}

auto GridCellIterator::operator!=(GridCellIterator other) const noexcept
    -> bool
{
    return !(*this == other);
}

auto GridCellIterator::operator*() noexcept -> graph::Node
{
    return cell_[idx_];
}
