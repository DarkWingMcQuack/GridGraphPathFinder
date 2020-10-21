#include <cmath>
#include <functional>
#include <graph/GridGraph.hpp>
#include <graph/GridGraphIterator.hpp>
#include <graph/Node.hpp>

using graph::GridGraphIterator;

GridGraphIterator::GridGraphIterator(const GridGraph& graph,
                                     std::size_t idx)
    : graph_(graph),
      idx_(idx),
      max_idx_(graph.getWidth() * graph.getHeight())
{
    auto node = getNodeAtIdx(idx_);
    while(graph_.get().isBarrier(node) && idx_ < max_idx_) {
        node = getNodeAtIdx(++idx_);
    }
}

auto GridGraphIterator::operator++() noexcept
    -> GridGraphIterator&
{
    auto node = getNodeAtIdx(++idx_);
    while(graph_.get().isBarrier(node) && idx_ < max_idx_) {
        node = getNodeAtIdx(++idx_);
    }
    return *this;
}

auto GridGraphIterator::operator++(int) noexcept
    -> GridGraphIterator
{
    auto ret = *this;
    ++(*this);
    return ret;
}

auto GridGraphIterator::operator+=(difference_type rhs) noexcept
    -> GridGraphIterator&
{
    idx_ += rhs;
    return *this;
}

auto GridGraphIterator::operator+=(const GridGraphIterator& rhs) noexcept
    -> GridGraphIterator&
{
    idx_ += rhs.idx_;
    return *this;
}

auto GridGraphIterator::operator==(const GridGraphIterator& other) const noexcept
    -> bool
{
    return idx_ == other.idx_;
}

auto GridGraphIterator::operator!=(const GridGraphIterator& other) const noexcept
    -> bool
{
    return idx_ != other.idx_;
}

auto GridGraphIterator::operator<=(const GridGraphIterator& other) const noexcept
    -> bool
{
    return idx_ <= other.idx_;
}

auto GridGraphIterator::operator>=(const GridGraphIterator& other) const noexcept
    -> bool
{
    return idx_ >= other.idx_;
}

auto GridGraphIterator::operator<(const GridGraphIterator& other) const noexcept
    -> bool
{
    return idx_ < other.idx_;
}

auto GridGraphIterator::operator>(const GridGraphIterator& other) const noexcept
    -> bool
{
    return idx_ > other.idx_;
}

auto GridGraphIterator::operator*() const noexcept
    -> graph::Node
{
    return getNodeAtIdx(idx_);
}

auto GridGraphIterator::operator->() const noexcept
    -> graph::Node
{
    return getNodeAtIdx(idx_);
}


auto GridGraphIterator::getNodeAtIdx(std::size_t idx) const noexcept
    -> graph::Node
{
    const auto width = graph_.get().getWidth();
    const auto row = static_cast<std::size_t>(
        std::floor(static_cast<double>(idx)
                   / static_cast<double>(width)));
    const auto column = idx % (width);

    return graph::Node{row, column};
}
