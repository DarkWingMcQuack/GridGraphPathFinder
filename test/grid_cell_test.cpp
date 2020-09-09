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

TEST(GridCellTest, SimpleSubSetSuperSetTest)
{
    GridCorner tl{0, 0};
    GridCorner tr{0, 3};
    GridCorner bl{3, 0};
    GridCorner br{3, 3};

    GridCell cell{tl, tr, bl, br};

    ASSERT_TRUE(cell.isValid());
    ASSERT_TRUE(cell.isSplitable());
    ASSERT_FALSE(cell.isAtomic());
    ASSERT_TRUE(cell.isSubSetOf(cell));
    ASSERT_TRUE(cell.isSuperSetOf(cell));

    auto [top_l, top_r, bottom_l, bottom_r] = cell.split();

    EXPECT_TRUE(top_l.isSubSetOf(cell));
    EXPECT_TRUE(top_r.isSubSetOf(cell));
    EXPECT_TRUE(bottom_l.isSubSetOf(cell));
    EXPECT_TRUE(bottom_r.isSubSetOf(cell));

    EXPECT_TRUE(cell.isSuperSetOf(top_l));
    EXPECT_TRUE(cell.isSuperSetOf(top_r));
    EXPECT_TRUE(cell.isSuperSetOf(bottom_l));
    EXPECT_TRUE(cell.isSuperSetOf(bottom_r));
}

TEST(GridCellTest, IndexTest)
{
    GridCorner tl{0, 0};
    GridCorner tr{0, 3};
    GridCorner bl{3, 0};
    GridCorner br{3, 3};

    GridCell cell{tl, tr, bl, br};

    ASSERT_TRUE(cell.isValid());
    ASSERT_TRUE(cell.isSplitable());
    ASSERT_FALSE(cell.isAtomic());

    auto n = cell[0];

    EXPECT_EQ(n.row, 0);
    EXPECT_EQ(n.column, 0);

    n = cell[1];
    EXPECT_EQ(n.row, 0);
    EXPECT_EQ(n.column, 1);

    n = cell[2];
    EXPECT_EQ(n.row, 0);
    EXPECT_EQ(n.column, 2);

    n = cell[3];
    EXPECT_EQ(n.row, 0);
    EXPECT_EQ(n.column, 3);

    n = cell[4];
    EXPECT_EQ(n.row, 1);
    EXPECT_EQ(n.column, 0);

    n = cell[5];
    EXPECT_EQ(n.row, 1);
    EXPECT_EQ(n.column, 1);

    n = cell[6];
    EXPECT_EQ(n.row, 1);
    EXPECT_EQ(n.column, 2);

    n = cell[7];
    EXPECT_EQ(n.row, 1);
    EXPECT_EQ(n.column, 3);

    n = cell[8];
    EXPECT_EQ(n.row, 2);
    EXPECT_EQ(n.column, 0);

    n = cell[9];
    EXPECT_EQ(n.row, 2);
    EXPECT_EQ(n.column, 1);

    n = cell[10];
    EXPECT_EQ(n.row, 2);
    EXPECT_EQ(n.column, 2);

    n = cell[11];
    EXPECT_EQ(n.row, 2);
    EXPECT_EQ(n.column, 3);

    n = cell[12];
    EXPECT_EQ(n.row, 3);
    EXPECT_EQ(n.column, 0);

    n = cell[13];
    EXPECT_EQ(n.row, 3);
    EXPECT_EQ(n.column, 1);

    n = cell[14];
    EXPECT_EQ(n.row, 3);
    EXPECT_EQ(n.column, 2);

    n = cell[15];
    EXPECT_EQ(n.row, 3);
    EXPECT_EQ(n.column, 3);

    n = cell[16];
    EXPECT_EQ(n.row, 4);
    EXPECT_EQ(n.column, 0);

    EXPECT_FALSE(cell.isInCell(n));
}
