#include <array>
#include <cmath>
#include <fmt/core.h>
#include <graph/GridCell.hpp>
#include <graph/GridCellIterator.hpp>
#include <graph/GridCorner.hpp>
#include <graph/Node.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <utils/Utils.hpp>

using graph::GridCell;
using graph::GridCorner;
using graph::GridCellIterator;
using graph::Node;
using utils::min;
using utils::max;


GridCell::GridCell(GridCorner top_left,
                   GridCorner bottom_right) noexcept
    : top_left_(top_left),
      bottom_right_(bottom_right) {}

auto GridCell::operator==(const GridCell& other) const noexcept
    -> bool
{
    return top_left_ == other.top_left_
        && bottom_right_ == other.bottom_right_;
}

auto GridCell::operator!=(const GridCell& other) const noexcept
    -> bool
{
    return !(*this == other);
}

auto GridCell::operator<(const GridCell& other) const noexcept
    -> bool
{
    return size() < other.size();
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

auto GridCell::isInCell(const graph::Node& node) const noexcept
    -> bool
{
    return node.row >= static_cast<std::size_t>(top_left_.row_)
        && node.row <= static_cast<std::size_t>(bottom_right_.row_)
        && node.column >= static_cast<std::size_t>(top_left_.column_)
        && node.column <= static_cast<std::size_t>(bottom_right_.column_);
}

auto GridCell::isSplitable() const noexcept
    -> bool
{
    const auto width = getWidth();
    const auto height = getHeight();

    return width >= 2
        && height >= 2
        && width % 2 == 0
        && height % 2 == 0;
}

auto GridCell::isAtomic() const noexcept
    -> bool
{
    const auto width = getWidth();
    const auto height = getHeight();

    return width == 1 && height == 1;
}

auto GridCell::isValid() const noexcept
    -> bool
{
    return bottom_right_.column_ >= top_left_.column_
        && top_left_.row_ <= bottom_right_.row_
        && bottom_right_.column_ >= top_left_.column_
        && top_left_.row_ >= 0
        && top_left_.column_ >= 0
        && bottom_right_.row_ >= 0
        && bottom_right_.column_ >= 0;
}


auto GridCell::to_string() const noexcept
    -> std::string
{
    std::stringstream ss;
    ss << "GridCell{"
       << top_left_
       << ", "
       << getTopRight()
       << ", "
       << getBottomLeft()
       << ", "
       << bottom_right_
       << "}";

    return ss.str();
}

auto GridCell::split() const noexcept
    -> std::array<GridCell, 4>
{
    auto current_top_right = getTopRight();
    auto current_bottom_left = getBottomLeft();

    const std::int64_t half_width = std::floor(static_cast<double>(current_top_right.column_ - top_left_.column_) / 2.);
    const std::int64_t half_hight = std::floor(static_cast<double>(current_bottom_left.row_ - top_left_.row_) / 2.);

    const auto top_left = GridCell{
        GridCorner{top_left_.row_, top_left_.column_},
        GridCorner{top_left_.row_ + half_hight, current_bottom_left.column_ + half_width}};

    const auto top_right = GridCell{
        GridCorner{top_left_.row_, top_left_.column_ + half_width + 1},
        GridCorner{top_left_.row_ + half_hight, current_top_right.column_}};

    const auto bottom_left = GridCell{
        GridCorner{top_left_.row_ + half_hight + 1, current_bottom_left.column_},
        GridCorner{bottom_right_.row_, top_left_.column_ + half_width}};

    const auto bottom_right = GridCell{
        GridCorner{top_left_.row_ + half_hight + 1, top_left_.column_ + half_width + 1},
        GridCorner{bottom_right_.row_, bottom_right_.column_}};

    return std::array{top_left,
                      top_right,
                      bottom_left,
                      bottom_right};
}

auto GridCell::getWidth() const noexcept
    -> std::size_t
{
    return bottom_right_.column_ - top_left_.column_ + 1;
}

auto GridCell::getHeight() const noexcept
    -> std::size_t
{
    return bottom_right_.row_ - top_left_.row_ + 1;
}


auto GridCell::getTopLeft() const noexcept
    -> GridCorner
{
    return top_left_;
}

auto GridCell::getTopRight() const noexcept
    -> GridCorner
{
    return GridCorner{top_left_.getRow(),
                      bottom_right_.getColumn()};
}

auto GridCell::getBottomLeft() const noexcept
    -> GridCorner
{
    return GridCorner{bottom_right_.getRow(),
                      top_left_.getColumn()};
}

auto GridCell::getBottomRight() const noexcept
    -> GridCorner
{
    return bottom_right_;
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


auto GridCell::cacluclateOrientation(const GridCell& other) const noexcept
    -> CellOrientation
{
    if(isSuperSetOf(other) or isSubSetOf(other)) {
        return CellOrientation::OTHER;
    }

    auto bottom_left = getBottomLeft();

    if(other.top_left_.row_ <= top_left_.row_
       and other.getBottomLeft().row_ >= bottom_left.row_) {
        return CellOrientation::HORIZONTAL;
    }

    if(top_left_.row_ <= other.top_left_.row_
       and bottom_left.row_ >= other.getBottomLeft().row_) {
        return CellOrientation::HORIZONTAL;
    }

    if(other.top_left_.column_ <= top_left_.column_
       and other.getBottomLeft().column_ >= bottom_left.column_) {
        return CellOrientation::VERTICAL;
    }

    if(top_left_.column_ <= other.top_left_.column_
       and bottom_left.column_ >= other.getBottomLeft().column_) {
        return CellOrientation::VERTICAL;
    }

    return CellOrientation::OTHER;
}

auto GridCell::isSuperSetOf(const GridCell& other) const noexcept
    -> bool
{
    return top_left_.row_ <= other.top_left_.row_
        and top_left_.column_ <= other.top_left_.column_
        and bottom_right_.row_ >= other.bottom_right_.row_
        and bottom_right_.column_ >= other.bottom_right_.column_;
}

auto GridCell::isSubSetOf(const GridCell& other) const noexcept
    -> bool
{
    return other.isSuperSetOf(*this);
}


auto GridCell::hasCommonNodeWith(const GridCell& other) const noexcept
    -> bool
{
    // TODO: make this in O(1)
    return std::any_of(std::begin(other),
                       std::end(other),
                       [&](auto node) {
                           return isInCell(node);
                       });
}




auto graph::operator<<(std::ostream& os, const GridCell& c) noexcept
    -> std::ostream&
{
    return os << "GridCell{"
              << c.top_left_
              << ", "
              << c.getTopRight()
              << ", "
              << c.getBottomLeft()
              << ", "
              << c.bottom_right_
              << "}";
}


auto graph::merge(const GridCell& first,
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
                    bottom_right};
}
