#include <GridCell.hpp>
#include <Node.hpp>
#include <array>

using grid::GridCell;
using grid::GridCorner;
using graph::Node;

GridCell::GridCell(GridCorner top_left,
                   GridCorner top_right,
                   GridCorner bottom_left,
                   GridCorner bottom_right) noexcept
    : top_left_(top_left),
      top_right_(top_right),
      bottom_left_(bottom_left),
      bottom_right_(bottom_right) {}

auto GridCell::operator==(const GridCell& other) const noexcept
    -> bool
{
    return top_left_ == other.top_left_
        && top_right_ == other.top_right_
        && bottom_left_ == other.bottom_left_
        && bottom_right_ == other.bottom_right_;
}

auto GridCell::operator!=(const GridCell& other) const noexcept
    -> bool
{
    return !(*this == other);
}

[[nodiscard]] auto GridCell::isInCell(const graph::Node& node) const noexcept
    -> bool
{
    return node.row >= top_left_.row
        && node.row <= bottom_left_.row
        && node.column >= top_left_.column
        && node.column <= top_right_.column;
}

[[nodiscard]] auto GridCell::isSplitable() const noexcept
    -> bool
{
    auto width = top_right_.column - top_left_.column;
    auto height = bottom_left_.row - top_left_.row;

    return width >= 2
        && height >= 2
        && width % 2 == 0
        && height % 2 == 0;
}

[[nodiscard]] auto GridCell::split() const noexcept
    -> std::array<GridCell, 4>
{
}
