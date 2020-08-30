#include <GridCell.hpp>
#include <GridCellIterator.hpp>
#include <Node.hpp>
#include <array>
#include <cmath>
#include <iostream>

using grid::GridCell;
using grid::GridCorner;
using grid::GridCellIterator;
using graph::Node;


auto GridCorner::operator==(const GridCorner& other) const noexcept
    -> bool
{
    return row_ == other.row_ && column_ == other.column_;
}

GridCorner::GridCorner(std::int64_t row,
                       std::int64_t column)
    : row_(row),
      column_(column) {}

auto GridCorner::operator!=(const GridCorner& other) const noexcept
    -> bool
{
    return !(*this == other);
}


auto GridCorner::getRow() const noexcept
    -> std::int64_t
{
    return row_;
}

auto GridCorner::getColumn() const noexcept
    -> std::int64_t
{
    return column_;
}

auto grid::operator<<(std::ostream& os, const GridCorner& c) noexcept
    -> std::ostream&
{
    return os << "("
              << std::to_string(c.row_)
              << ", "
              << std::to_string(c.column_)
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
    return node.row >= top_left_.row_
        && node.row <= bottom_left_.row_
        && node.column >= top_left_.column_
        && node.column <= top_right_.column_;
}

[[nodiscard]] auto GridCell::isSplitable() const noexcept
    -> bool
{
    const auto width = getWidth();
    const auto height = getHeight();

    return width >= 2
        && height >= 2
        && width % 2 == 0
        && height % 2 == 0;
}

[[nodiscard]] auto GridCell::isAtomic() const noexcept
    -> bool
{
    const auto width = getWidth();
    const auto height = getHeight();

    return width == 1 && height == 1;
}

[[nodiscard]] auto GridCell::isValid() const noexcept
    -> bool
{
    return top_right_.row_ == top_left_.row_
        && top_right_.column_ >= top_left_.column_
        && top_left_.row_ <= bottom_left_.row_
        && bottom_left_.row_ == bottom_right_.row_
        && bottom_right_.column_ >= bottom_left_.column_
        && top_left_.row_ >= 0 && top_left_.column_ >= 0
        && top_right_.row_ >= 0 && top_right_.column_ >= 0
        && bottom_left_.row_ >= 0 && bottom_left_.column_ >= 0
        && bottom_right_.row_ >= 0 && bottom_right_.column_ >= 0;
}

[[nodiscard]] auto GridCell::split() const noexcept
    -> std::array<GridCell, 4>
{
    const std::int64_t half_width = std::floor(top_right_.column_ / 2.);
    const std::int64_t half_hight = std::floor(bottom_left_.row_ / 2.);

    const auto top_left = GridCell{
        GridCorner{top_left_.row_, top_left_.column_},
        GridCorner{top_left_.row_, top_right_.column_ / 2},
        GridCorner{half_hight, top_left_.column_},
        GridCorner{half_hight, half_width}};

    const auto top_right = GridCell{
        GridCorner{top_left_.row_, half_width + 1},
        GridCorner{top_right_.row_, top_right_.column_},
        GridCorner{half_hight, half_width + 1},
        GridCorner{half_hight, top_right_.column_}};

    const auto bottom_left = GridCell{
        GridCorner{half_hight + 1, bottom_left_.column_},
        GridCorner{half_hight + 1, half_width},
        GridCorner{bottom_left_.row_, bottom_left_.column_},
        GridCorner{bottom_right_.row_, half_width}};

    const auto bottom_right = GridCell{
        GridCorner{half_hight + 1, half_width + 1},
        GridCorner{half_hight + 1, bottom_right_.column_},
        GridCorner{bottom_left_.row_, half_width + 1},
        GridCorner{bottom_right_.row_, bottom_right_.column_}};

    return std::array{
        std::move(top_left),
        std::move(top_right),
        std::move(bottom_left),
        std::move(bottom_right)};
}

[[nodiscard]] auto GridCell::getWidth() const noexcept
    -> std::size_t
{
    return top_right_.column_ - top_left_.column_ + 1;
}

[[nodiscard]] auto GridCell::getHeight() const noexcept
    -> std::size_t
{
    return bottom_left_.row_ - top_left_.row_ + 1;
}

auto GridCell::getNodeAt(std::size_t row, std::size_t column) const noexcept
    -> std::optional<graph::Node>
{
    const auto width = getWidth();
    const auto height = getHeight();

    if(width <= column || height <= column) {
        return std::nullopt;
    }

    return graph::Node{row + top_left_.row_,
                       column + top_left_.column_};
}

auto GridCell::begin() const noexcept
    -> GridCellIterator
{
    return GridCellIterator{*this};
}

auto GridCell::end() const noexcept
    -> GridCellIterator
{
    return GridCellIterator{*this,
                            getWidth() * getHeight()};
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
