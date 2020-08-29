#include <GridCell.hpp>
#include <Node.hpp>
#include <array>
#include <cmath>
#include <iostream>

using grid::GridCell;
using grid::GridCorner;
using graph::Node;


auto GridCorner::operator==(const GridCorner& other) const noexcept
    -> bool
{
    return row == other.row && column == other.column;
}

auto GridCorner::operator!=(const GridCorner& other) const noexcept
    -> bool
{
    return !(*this == other);
}

auto grid::operator<<(std::ostream& os, const GridCorner& c) noexcept
    -> std::ostream&
{
    return os << "("
              << std::to_string(c.row)
              << ", "
              << std::to_string(c.column)
              << ")";
}

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
    const auto width = top_right_.column - top_left_.column + 1;
    const auto height = bottom_left_.row - top_left_.row + 1;

    return width >= 2
        && height >= 2
        && width % 2 == 0
        && height % 2 == 0;
}

[[nodiscard]] auto GridCell::isAtomic() const noexcept
    -> bool
{
    const auto width = top_right_.column - top_left_.column + 1;
    const auto height = bottom_left_.row - top_left_.row + 1;

    return width == 1 && height == 1;
}

[[nodiscard]] auto GridCell::isValid() const noexcept
    -> bool
{
    return top_right_.row == top_left_.row
        && top_right_.column >= top_left_.column
        && top_left_.row <= bottom_left_.row
        && bottom_left_.row == bottom_right_.row
        && bottom_right_.column >= bottom_left_.column
        && top_left_.row >= 0 && top_left_.column >= 0
        && top_right_.row >= 0 && top_right_.column >= 0
        && bottom_left_.row >= 0 && bottom_left_.column >= 0
        && bottom_right_.row >= 0 && bottom_right_.column >= 0;
}

[[nodiscard]] auto GridCell::split() const noexcept
    -> std::array<GridCell, 4>
{
    const std::int64_t half_width = std::floor(top_right_.column / 2.);
    const std::int64_t half_hight = std::floor(bottom_left_.row / 2.);

    const auto top_left = GridCell{
        GridCorner{top_left_.row, top_left_.column},
        GridCorner{top_left_.row, top_right_.column / 2},
        GridCorner{half_hight, top_left_.column},
        GridCorner{half_hight, half_width}};

    const auto top_right = GridCell{
        GridCorner{top_left_.row, half_width + 1},
        GridCorner{top_right_.row, top_right_.column},
        GridCorner{half_hight, half_width + 1},
        GridCorner{half_hight, top_right_.column}};

    const auto bottom_left = GridCell{
        GridCorner{half_hight + 1, bottom_left_.column},
        GridCorner{half_hight + 1, half_width},
        GridCorner{bottom_left_.row, bottom_left_.column},
        GridCorner{bottom_right_.row, half_width}};

    const auto bottom_right = GridCell{
        GridCorner{half_hight + 1, half_width + 1},
        GridCorner{half_hight + 1, bottom_right_.column},
        GridCorner{bottom_left_.row, half_width + 1},
        GridCorner{bottom_right_.row, bottom_right_.column}};

    return std::array{
        std::move(top_left),
        std::move(top_right),
        std::move(bottom_left),
        std::move(bottom_right)};
}


auto grid::operator<<(std::ostream& os, const GridCell& c) noexcept
    -> std::ostream&
{
    return os << "GridCell{"
              << c.top_left_
              << ", "
              << c.top_right_
              << ", "
              << c.bottom_left_
              << ", "
              << c.bottom_right_
              << "}";
}
