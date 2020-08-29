#include <GridCell.hpp>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

#include <gtest/gtest.h>

using grid::GridCell;
using grid::GridCorner;


TEST(GridCellTest, SimpleSplitTest)
{
    GridCorner tl{0, 0};
    GridCorner tr{0, 1};
    GridCorner bl{1, 0};
    GridCorner br{1, 1};

    GridCell cell{tl, tr, bl, br};

    ASSERT_TRUE(cell.isValid());
    ASSERT_TRUE(cell.isSplitable());
    ASSERT_FALSE(cell.isAtomic());

    auto [top_l, top_r, bottom_l, bottom_r] = cell.split();

    EXPECT_TRUE(top_l.isValid());
    EXPECT_TRUE(top_r.isValid());
    EXPECT_TRUE(bottom_l.isValid());
    EXPECT_TRUE(bottom_r.isValid());

    EXPECT_TRUE(top_l.isAtomic());
    EXPECT_TRUE(top_r.isAtomic());
    EXPECT_TRUE(bottom_l.isAtomic());
    EXPECT_TRUE(bottom_r.isAtomic());
}

TEST(GridCellTest, AnotherSimpleSplitTest)
{
    GridCorner tl{0, 0};
    GridCorner tr{0, 3};
    GridCorner bl{3, 0};
    GridCorner br{3, 3};

    GridCell cell{tl, tr, bl, br};

    ASSERT_TRUE(cell.isValid());
    ASSERT_TRUE(cell.isSplitable());
    ASSERT_FALSE(cell.isAtomic());

    auto [top_l, top_r, bottom_l, bottom_r] = cell.split();

    EXPECT_EQ(top_l, GridCell(GridCorner(0, 0), GridCorner(0, 1), GridCorner(1, 0), GridCorner(1, 1)));
    EXPECT_EQ(top_r, GridCell(GridCorner(0, 2), GridCorner(0, 3), GridCorner(1, 2), GridCorner(1, 3)));
    EXPECT_EQ(bottom_l, GridCell(GridCorner(2, 0), GridCorner(2, 1), GridCorner(3, 0), GridCorner(3, 1)));
    EXPECT_EQ(bottom_r, GridCell(GridCorner(2, 2), GridCorner(2, 3), GridCorner(3, 2), GridCorner(3, 3)));

    EXPECT_TRUE(top_l.isValid());
    EXPECT_TRUE(top_r.isValid());
    EXPECT_TRUE(bottom_l.isValid());
    EXPECT_TRUE(bottom_r.isValid());

    EXPECT_FALSE(top_l.isAtomic());
    EXPECT_FALSE(top_r.isAtomic());
    EXPECT_FALSE(bottom_l.isAtomic());
    EXPECT_FALSE(bottom_r.isAtomic());

    EXPECT_TRUE(top_l.isSplitable());
    EXPECT_TRUE(top_r.isSplitable());
    EXPECT_TRUE(bottom_l.isSplitable());
    EXPECT_TRUE(bottom_r.isSplitable());
}
