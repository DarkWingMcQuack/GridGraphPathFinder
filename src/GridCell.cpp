#include <GridCell.hpp>
#include <GridCellIterator.hpp>
#include <GridCorner.hpp>
#include <Node.hpp>
#include <Utils.hpp>
#include <array>
#include <cmath>
#include <iostream>

using grid::GridCell;
using grid::GridCorner;
using grid::GridCellIterator;
using graph::Node;
using util::min;
using util::max;


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

auto GridCell::operator[](std::size_t idx) const noexcept
    -> graph::Node
{
    const auto width = getWidth();
    const auto row = static_cast<std::size_t>(
        std::floor(static_cast<double>(idx)
                   / static_cast<double>(width)));
    const auto column = idx % (width);

    return graph::Node{row + top_left_.row_,
                       column + top_left_.column_};
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

    return std::array{top_left,
                      top_right,
                      bottom_left,
                      bottom_right};
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
    auto end_index = static_cast<GridCellIterator::difference_type>(getWidth() * getHeight());
    return GridCellIterator{*this, end_index};
}

auto GridCell::size() const noexcept
    -> std::size_t
{
    return getHeight() * getWidth();
}

auto GridCell::isSuperSetOf(const GridCell& other) const noexcept
    -> bool
{
    return top_left_.row_ <= other.top_left_.row_
        && top_left_.column_ <= other.top_left_.column_
        && bottom_left_.row_ >= other.bottom_left_.row_
        && bottom_left_.column_ <= other.bottom_left_.column_
        && bottom_right_.row_ >= other.bottom_right_.row_
        && bottom_right_.column_ >= other.bottom_right_.column_
        && top_right_.row_ <= other.top_right_.row_
        && top_right_.column_ >= other.top_right_.column_;
}

auto GridCell::isSubSetOf(const GridCell& other) const noexcept
    -> bool
{
    return top_left_.row_ >= other.top_left_.row_
        && top_left_.column_ >= other.top_left_.column_
        && bottom_left_.row_ <= other.bottom_left_.row_
        && bottom_left_.column_ >= other.bottom_left_.column_
        && bottom_right_.row_ <= other.bottom_right_.row_
        && bottom_right_.column_ <= other.bottom_right_.column_
        && top_right_.row_ >= other.top_right_.row_
        && top_right_.column_ <= other.top_right_.column_;
}


auto GridCell::hasCommonNodeWith(const GridCell& other) const noexcept
    -> bool
{
    return std::any_of(std::begin(other),
                       std::end(other),
                       [](auto node) {
                           return isInCell(node);
                       });
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


auto grid::merge(const GridCell& first,
                 const GridCell& second,
                 const GridCell& third,
                 const GridCell& fourth) noexcept
    -> GridCell
{
    GridCorner top_left{
        min(first.top_left_.getRow(),
            second.top_left_.getRow(),
            third.top_left_.getRow(),
            fourth.top_left_.getRow()),
        min(first.top_left_.getColumn(),
            second.top_left_.getColumn(),
            third.top_left_.getColumn(),
            fourth.top_left_.getColumn())};

    GridCorner top_right{
        min(first.top_right_.getRow(),
            second.top_right_.getRow(),
            third.top_right_.getRow(),
            fourth.top_right_.getRow()),
        max(first.top_right_.getColumn(),
            second.top_right_.getColumn(),
            third.top_right_.getColumn(),
            fourth.top_right_.getColumn())};

    GridCorner bottom_left{
        max(first.bottom_left_.getRow(),
            second.bottom_left_.getRow(),
            third.bottom_left_.getRow(),
            fourth.bottom_left_.getRow()),
        min(first.bottom_left_.getColumn(),
            second.bottom_left_.getColumn(),
            third.bottom_left_.getColumn(),
            fourth.bottom_left_.getColumn())};

    GridCorner bottom_right{
        max(first.bottom_right_.getRow(),
            second.bottom_right_.getRow(),
            third.bottom_right_.getRow(),
            fourth.bottom_right_.getRow()),
        max(first.bottom_right_.getColumn(),
            second.bottom_right_.getColumn(),
            third.bottom_right_.getColumn(),
            fourth.bottom_right_.getColumn())};

    return GridCell{top_left,
                    top_right,
                    bottom_left,
                    bottom_right};
}
