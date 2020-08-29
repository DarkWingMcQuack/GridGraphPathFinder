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
    const auto width = top_right_.column - top_left_.column;
    const auto height = bottom_left_.row - top_left_.row;

    return width >= 2
        && height >= 2
        && width % 2 == 0
        && height % 2 == 0;
}

[[nodiscard]] auto GridCell::isAtomic() const noexcept
    -> bool
{
    const auto width = top_right_.column - top_left_.column;
    const auto height = bottom_left_.row - top_left_.row;

    return width == 1 && height == 2;
}

[[nodiscard]] auto GridCell::split() const noexcept
    -> std::array<GridCell, 4>
{
    const auto top_left = GridCell{
        GridCorner{top_left_.row, top_left_.column},
        GridCorner{top_left_.row, top_right_.column / 2},
        GridCorner{bottom_left_.row / 2, top_left_.column},
        GridCorner{bottom_left_.row / 2, top_right_.column / 2}};

    const auto top_right = GridCell{
        GridCorner{top_left_.row, top_right_.column / 2 + 1},
        GridCorner{top_right_.row, top_right_.column},
        GridCorner{bottom_left_.row / 2, top_right_.column / 2 + 1},
        GridCorner{bottom_right_.row / 2, top_right_.column}};

    const auto bottom_left = GridCell{
        GridCorner{bottom_left_.row / 2 + 1, bottom_right_.column},
        GridCorner{bottom_left_.row / 2 + 1, bottom_right_.column / 2},
        GridCorner{bottom_left_.row, bottom_left_.column},
        GridCorner{bottom_right_.row, bottom_right_.column / 2}};

    const auto bottom_right = GridCell{
        GridCorner{bottom_left_.row / 2 + 1, bottom_right_.column / 2 + 1},
        GridCorner{bottom_left_.row / 2 + 1, bottom_right_.column},
        GridCorner{bottom_left_.row, bottom_right_.column / 2 + 1},
        GridCorner{bottom_right_.row, bottom_right_.column}};

    return std::array{
        std::move(top_left),
        std::move(top_right),
        std::move(bottom_left),
        std::move(bottom_right)};
}
