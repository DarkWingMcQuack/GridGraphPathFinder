#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <graph/GridGraph.hpp>

#include <gtest/gtest.h>

using graph::Node;
using graph::GridGraph;

TEST(GridGraphTest, SimpleSquareGridGraphTest)
{
    std::vector test1{
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1, graph::ManhattanNeigbourCalculator{}};

    EXPECT_EQ(graph_test1.getWidth(), 5);
    EXPECT_EQ(graph_test1.getHeight(), 5);

    EXPECT_TRUE(graph_test1.isWalkableNode({0, 0}));
    EXPECT_TRUE(graph_test1.isWalkableNode({0, 1}));
    EXPECT_TRUE(graph_test1.isWalkableNode({0, 2}));
    EXPECT_TRUE(graph_test1.isWalkableNode({0, 3}));
    EXPECT_TRUE(graph_test1.isWalkableNode({0, 4}));
    EXPECT_TRUE(graph_test1.isWalkableNode({1, 0}));
    EXPECT_TRUE(graph_test1.isWalkableNode({1, 1}));
    EXPECT_TRUE(graph_test1.isWalkableNode({1, 2}));
    EXPECT_TRUE(graph_test1.isWalkableNode({1, 3}));
    EXPECT_TRUE(graph_test1.isWalkableNode({1, 4}));
    EXPECT_TRUE(graph_test1.isWalkableNode({2, 0}));
    EXPECT_TRUE(graph_test1.isWalkableNode({2, 1}));
    EXPECT_TRUE(graph_test1.isWalkableNode({2, 2}));
    EXPECT_TRUE(graph_test1.isWalkableNode({2, 3}));
    EXPECT_TRUE(graph_test1.isWalkableNode({2, 4}));
    EXPECT_TRUE(graph_test1.isWalkableNode({3, 0}));
    EXPECT_TRUE(graph_test1.isWalkableNode({3, 1}));
    EXPECT_TRUE(graph_test1.isWalkableNode({3, 2}));
    EXPECT_TRUE(graph_test1.isWalkableNode({3, 3}));
    EXPECT_TRUE(graph_test1.isWalkableNode({3, 4}));
    EXPECT_TRUE(graph_test1.isWalkableNode({4, 0}));
    EXPECT_TRUE(graph_test1.isWalkableNode({4, 1}));
    EXPECT_TRUE(graph_test1.isWalkableNode({4, 2}));
    EXPECT_TRUE(graph_test1.isWalkableNode({4, 3}));
    EXPECT_TRUE(graph_test1.isWalkableNode({4, 4}));

    EXPECT_FALSE(graph_test1.isBarrier({0, 0}));
    EXPECT_FALSE(graph_test1.isBarrier({0, 1}));
    EXPECT_FALSE(graph_test1.isBarrier({0, 2}));
    EXPECT_FALSE(graph_test1.isBarrier({0, 3}));
    EXPECT_FALSE(graph_test1.isBarrier({0, 4}));
    EXPECT_FALSE(graph_test1.isBarrier({1, 0}));
    EXPECT_FALSE(graph_test1.isBarrier({1, 1}));
    EXPECT_FALSE(graph_test1.isBarrier({1, 2}));
    EXPECT_FALSE(graph_test1.isBarrier({1, 3}));
    EXPECT_FALSE(graph_test1.isBarrier({1, 4}));
    EXPECT_FALSE(graph_test1.isBarrier({2, 0}));
    EXPECT_FALSE(graph_test1.isBarrier({2, 1}));
    EXPECT_FALSE(graph_test1.isBarrier({2, 2}));
    EXPECT_FALSE(graph_test1.isBarrier({2, 3}));
    EXPECT_FALSE(graph_test1.isBarrier({2, 4}));
    EXPECT_FALSE(graph_test1.isBarrier({3, 0}));
    EXPECT_FALSE(graph_test1.isBarrier({3, 1}));
    EXPECT_FALSE(graph_test1.isBarrier({3, 2}));
    EXPECT_FALSE(graph_test1.isBarrier({3, 3}));
    EXPECT_FALSE(graph_test1.isBarrier({3, 4}));
    EXPECT_FALSE(graph_test1.isBarrier({4, 0}));
    EXPECT_FALSE(graph_test1.isBarrier({4, 1}));
    EXPECT_FALSE(graph_test1.isBarrier({4, 2}));
    EXPECT_FALSE(graph_test1.isBarrier({4, 3}));
    EXPECT_FALSE(graph_test1.isBarrier({4, 4}));

    EXPECT_FALSE(graph_test1.isWalkableNode({static_cast<std::size_t>(-1), 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, static_cast<std::size_t>(-1)}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, 5}));
    EXPECT_FALSE(graph_test1.isWalkableNode({5, 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({5, 5}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, 6}));
    EXPECT_FALSE(graph_test1.isWalkableNode({6, 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({6, 6}));

    EXPECT_TRUE(graph_test1.isBarrier({static_cast<std::size_t>(-1), 0}));
    EXPECT_TRUE(graph_test1.isBarrier({0, static_cast<std::size_t>(-1)}));
    EXPECT_TRUE(graph_test1.isBarrier({0, 5}));
    EXPECT_TRUE(graph_test1.isBarrier({5, 0}));
    EXPECT_TRUE(graph_test1.isBarrier({5, 5}));
    EXPECT_TRUE(graph_test1.isBarrier({0, 6}));
    EXPECT_TRUE(graph_test1.isBarrier({6, 0}));
    EXPECT_TRUE(graph_test1.isBarrier({6, 6}));
}

TEST(GridGraphTest, SimpleNonSquareGridGraphTest)
{
    std::vector test1{
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1, graph::ManhattanNeigbourCalculator{}};

    EXPECT_EQ(graph_test1.getWidth(), 5);
    EXPECT_EQ(graph_test1.getHeight(), 4);

    EXPECT_TRUE(graph_test1.isWalkableNode({0, 0}));
    EXPECT_TRUE(graph_test1.isWalkableNode({0, 1}));
    EXPECT_TRUE(graph_test1.isWalkableNode({0, 2}));
    EXPECT_TRUE(graph_test1.isWalkableNode({0, 3}));
    EXPECT_TRUE(graph_test1.isWalkableNode({0, 4}));
    EXPECT_TRUE(graph_test1.isWalkableNode({1, 0}));
    EXPECT_TRUE(graph_test1.isWalkableNode({1, 1}));
    EXPECT_TRUE(graph_test1.isWalkableNode({1, 2}));
    EXPECT_TRUE(graph_test1.isWalkableNode({1, 3}));
    EXPECT_TRUE(graph_test1.isWalkableNode({1, 4}));
    EXPECT_TRUE(graph_test1.isWalkableNode({2, 0}));
    EXPECT_TRUE(graph_test1.isWalkableNode({2, 1}));
    EXPECT_TRUE(graph_test1.isWalkableNode({2, 2}));
    EXPECT_TRUE(graph_test1.isWalkableNode({2, 3}));
    EXPECT_TRUE(graph_test1.isWalkableNode({2, 4}));
    EXPECT_TRUE(graph_test1.isWalkableNode({3, 0}));
    EXPECT_TRUE(graph_test1.isWalkableNode({3, 1}));
    EXPECT_TRUE(graph_test1.isWalkableNode({3, 2}));
    EXPECT_TRUE(graph_test1.isWalkableNode({3, 3}));
    EXPECT_TRUE(graph_test1.isWalkableNode({3, 4}));

    EXPECT_FALSE(graph_test1.isBarrier({0, 0}));
    EXPECT_FALSE(graph_test1.isBarrier({0, 1}));
    EXPECT_FALSE(graph_test1.isBarrier({0, 2}));
    EXPECT_FALSE(graph_test1.isBarrier({0, 3}));
    EXPECT_FALSE(graph_test1.isBarrier({0, 4}));
    EXPECT_FALSE(graph_test1.isBarrier({1, 0}));
    EXPECT_FALSE(graph_test1.isBarrier({1, 1}));
    EXPECT_FALSE(graph_test1.isBarrier({1, 2}));
    EXPECT_FALSE(graph_test1.isBarrier({1, 3}));
    EXPECT_FALSE(graph_test1.isBarrier({1, 4}));
    EXPECT_FALSE(graph_test1.isBarrier({2, 0}));
    EXPECT_FALSE(graph_test1.isBarrier({2, 1}));
    EXPECT_FALSE(graph_test1.isBarrier({2, 2}));
    EXPECT_FALSE(graph_test1.isBarrier({2, 3}));
    EXPECT_FALSE(graph_test1.isBarrier({2, 4}));
    EXPECT_FALSE(graph_test1.isBarrier({3, 0}));
    EXPECT_FALSE(graph_test1.isBarrier({3, 1}));
    EXPECT_FALSE(graph_test1.isBarrier({3, 2}));
    EXPECT_FALSE(graph_test1.isBarrier({3, 3}));
    EXPECT_FALSE(graph_test1.isBarrier({3, 4}));

    EXPECT_FALSE(graph_test1.isWalkableNode({static_cast<std::size_t>(-1), 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, static_cast<std::size_t>(-1)}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, 5}));
    EXPECT_FALSE(graph_test1.isWalkableNode({5, 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({5, 5}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, 6}));
    EXPECT_FALSE(graph_test1.isWalkableNode({6, 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({6, 6}));
    EXPECT_FALSE(graph_test1.isWalkableNode({4, 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({4, 1}));
    EXPECT_FALSE(graph_test1.isWalkableNode({4, 2}));
    EXPECT_FALSE(graph_test1.isWalkableNode({4, 3}));
    EXPECT_FALSE(graph_test1.isWalkableNode({4, 4}));

    EXPECT_TRUE(graph_test1.isBarrier({static_cast<std::size_t>(-1), 0}));
    EXPECT_TRUE(graph_test1.isBarrier({0, static_cast<std::size_t>(-1)}));
    EXPECT_TRUE(graph_test1.isBarrier({0, 5}));
    EXPECT_TRUE(graph_test1.isBarrier({5, 0}));
    EXPECT_TRUE(graph_test1.isBarrier({5, 5}));
    EXPECT_TRUE(graph_test1.isBarrier({0, 6}));
    EXPECT_TRUE(graph_test1.isBarrier({6, 0}));
    EXPECT_TRUE(graph_test1.isBarrier({6, 6}));
    EXPECT_TRUE(graph_test1.isBarrier({4, 0}));
    EXPECT_TRUE(graph_test1.isBarrier({4, 1}));
    EXPECT_TRUE(graph_test1.isBarrier({4, 2}));
    EXPECT_TRUE(graph_test1.isBarrier({4, 3}));
    EXPECT_TRUE(graph_test1.isBarrier({4, 4}));
}

TEST(GridGraphTest, SimpleBarrierSquareGridGraphTest)
{
    std::vector test1{
        std::vector{false, false, false, false, false},
        std::vector{false, false, false, false, false},
        std::vector{false, false, false, false, false},
        std::vector{false, false, false, false, false},
        std::vector{false, false, false, false, false}};

    GridGraph graph_test1{test1, graph::ManhattanNeigbourCalculator{}};

    EXPECT_EQ(graph_test1.getWidth(), 0);
    EXPECT_EQ(graph_test1.getHeight(), 0);

    EXPECT_FALSE(graph_test1.isWalkableNode({0, 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, 1}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, 2}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, 3}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, 4}));
    EXPECT_FALSE(graph_test1.isWalkableNode({1, 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({1, 1}));
    EXPECT_FALSE(graph_test1.isWalkableNode({1, 2}));
    EXPECT_FALSE(graph_test1.isWalkableNode({1, 3}));
    EXPECT_FALSE(graph_test1.isWalkableNode({1, 4}));
    EXPECT_FALSE(graph_test1.isWalkableNode({2, 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({2, 1}));
    EXPECT_FALSE(graph_test1.isWalkableNode({2, 2}));
    EXPECT_FALSE(graph_test1.isWalkableNode({2, 3}));
    EXPECT_FALSE(graph_test1.isWalkableNode({2, 4}));
    EXPECT_FALSE(graph_test1.isWalkableNode({3, 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({3, 1}));
    EXPECT_FALSE(graph_test1.isWalkableNode({3, 2}));
    EXPECT_FALSE(graph_test1.isWalkableNode({3, 3}));
    EXPECT_FALSE(graph_test1.isWalkableNode({3, 4}));
    EXPECT_FALSE(graph_test1.isWalkableNode({4, 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({4, 1}));
    EXPECT_FALSE(graph_test1.isWalkableNode({4, 2}));
    EXPECT_FALSE(graph_test1.isWalkableNode({4, 3}));
    EXPECT_FALSE(graph_test1.isWalkableNode({4, 4}));

    EXPECT_TRUE(graph_test1.isBarrier({0, 0}));
    EXPECT_TRUE(graph_test1.isBarrier({0, 1}));
    EXPECT_TRUE(graph_test1.isBarrier({0, 2}));
    EXPECT_TRUE(graph_test1.isBarrier({0, 3}));
    EXPECT_TRUE(graph_test1.isBarrier({0, 4}));
    EXPECT_TRUE(graph_test1.isBarrier({1, 0}));
    EXPECT_TRUE(graph_test1.isBarrier({1, 1}));
    EXPECT_TRUE(graph_test1.isBarrier({1, 2}));
    EXPECT_TRUE(graph_test1.isBarrier({1, 3}));
    EXPECT_TRUE(graph_test1.isBarrier({1, 4}));
    EXPECT_TRUE(graph_test1.isBarrier({2, 0}));
    EXPECT_TRUE(graph_test1.isBarrier({2, 1}));
    EXPECT_TRUE(graph_test1.isBarrier({2, 2}));
    EXPECT_TRUE(graph_test1.isBarrier({2, 3}));
    EXPECT_TRUE(graph_test1.isBarrier({2, 4}));
    EXPECT_TRUE(graph_test1.isBarrier({3, 0}));
    EXPECT_TRUE(graph_test1.isBarrier({3, 1}));
    EXPECT_TRUE(graph_test1.isBarrier({3, 2}));
    EXPECT_TRUE(graph_test1.isBarrier({3, 3}));
    EXPECT_TRUE(graph_test1.isBarrier({3, 4}));
    EXPECT_TRUE(graph_test1.isBarrier({4, 0}));
    EXPECT_TRUE(graph_test1.isBarrier({4, 1}));
    EXPECT_TRUE(graph_test1.isBarrier({4, 2}));
    EXPECT_TRUE(graph_test1.isBarrier({4, 3}));
    EXPECT_TRUE(graph_test1.isBarrier({4, 4}));

    EXPECT_FALSE(graph_test1.isWalkableNode({static_cast<std::size_t>(-1), 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, static_cast<std::size_t>(-1)}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, 5}));
    EXPECT_FALSE(graph_test1.isWalkableNode({5, 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({5, 5}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, 6}));
    EXPECT_FALSE(graph_test1.isWalkableNode({6, 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({6, 6}));

    EXPECT_TRUE(graph_test1.isBarrier({static_cast<std::size_t>(-1), 0}));
    EXPECT_TRUE(graph_test1.isBarrier({0, static_cast<std::size_t>(-1)}));
    EXPECT_TRUE(graph_test1.isBarrier({0, 5}));
    EXPECT_TRUE(graph_test1.isBarrier({5, 0}));
    EXPECT_TRUE(graph_test1.isBarrier({5, 5}));
    EXPECT_TRUE(graph_test1.isBarrier({0, 6}));
    EXPECT_TRUE(graph_test1.isBarrier({6, 0}));
    EXPECT_TRUE(graph_test1.isBarrier({6, 6}));
}

TEST(GridGraphTest, BarrierSquareGridGraphTest)
{
    std::vector test1{
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, false, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1, graph::ManhattanNeigbourCalculator{}};

    EXPECT_EQ(graph_test1.getWidth(), 5);
    EXPECT_EQ(graph_test1.getHeight(), 5);

    EXPECT_TRUE(graph_test1.isWalkableNode({0, 0}));
    EXPECT_TRUE(graph_test1.isWalkableNode({0, 1}));
    EXPECT_TRUE(graph_test1.isWalkableNode({0, 2}));
    EXPECT_TRUE(graph_test1.isWalkableNode({0, 3}));
    EXPECT_TRUE(graph_test1.isWalkableNode({0, 4}));
    EXPECT_TRUE(graph_test1.isWalkableNode({1, 0}));
    EXPECT_TRUE(graph_test1.isWalkableNode({1, 1}));
    EXPECT_TRUE(graph_test1.isWalkableNode({1, 2}));
    EXPECT_TRUE(graph_test1.isWalkableNode({1, 3}));
    EXPECT_TRUE(graph_test1.isWalkableNode({1, 4}));
    EXPECT_TRUE(graph_test1.isWalkableNode({2, 0}));
    EXPECT_TRUE(graph_test1.isWalkableNode({2, 1}));

    EXPECT_FALSE(graph_test1.isWalkableNode({2, 2}));

    EXPECT_TRUE(graph_test1.isWalkableNode({2, 3}));
    EXPECT_TRUE(graph_test1.isWalkableNode({2, 4}));
    EXPECT_TRUE(graph_test1.isWalkableNode({3, 0}));
    EXPECT_TRUE(graph_test1.isWalkableNode({3, 1}));
    EXPECT_TRUE(graph_test1.isWalkableNode({3, 2}));
    EXPECT_TRUE(graph_test1.isWalkableNode({3, 3}));
    EXPECT_TRUE(graph_test1.isWalkableNode({3, 4}));
    EXPECT_TRUE(graph_test1.isWalkableNode({4, 0}));
    EXPECT_TRUE(graph_test1.isWalkableNode({4, 1}));
    EXPECT_TRUE(graph_test1.isWalkableNode({4, 2}));
    EXPECT_TRUE(graph_test1.isWalkableNode({4, 3}));
    EXPECT_TRUE(graph_test1.isWalkableNode({4, 4}));

    EXPECT_FALSE(graph_test1.isBarrier({0, 0}));
    EXPECT_FALSE(graph_test1.isBarrier({0, 1}));
    EXPECT_FALSE(graph_test1.isBarrier({0, 2}));
    EXPECT_FALSE(graph_test1.isBarrier({0, 3}));
    EXPECT_FALSE(graph_test1.isBarrier({0, 4}));
    EXPECT_FALSE(graph_test1.isBarrier({1, 0}));
    EXPECT_FALSE(graph_test1.isBarrier({1, 1}));
    EXPECT_FALSE(graph_test1.isBarrier({1, 2}));
    EXPECT_FALSE(graph_test1.isBarrier({1, 3}));
    EXPECT_FALSE(graph_test1.isBarrier({1, 4}));
    EXPECT_FALSE(graph_test1.isBarrier({2, 0}));
    EXPECT_FALSE(graph_test1.isBarrier({2, 1}));

    EXPECT_TRUE(graph_test1.isBarrier({2, 2}));

    EXPECT_FALSE(graph_test1.isBarrier({2, 3}));
    EXPECT_FALSE(graph_test1.isBarrier({2, 4}));
    EXPECT_FALSE(graph_test1.isBarrier({3, 0}));
    EXPECT_FALSE(graph_test1.isBarrier({3, 1}));
    EXPECT_FALSE(graph_test1.isBarrier({3, 2}));
    EXPECT_FALSE(graph_test1.isBarrier({3, 3}));
    EXPECT_FALSE(graph_test1.isBarrier({3, 4}));
    EXPECT_FALSE(graph_test1.isBarrier({4, 0}));
    EXPECT_FALSE(graph_test1.isBarrier({4, 1}));
    EXPECT_FALSE(graph_test1.isBarrier({4, 2}));
    EXPECT_FALSE(graph_test1.isBarrier({4, 3}));
    EXPECT_FALSE(graph_test1.isBarrier({4, 4}));

    EXPECT_FALSE(graph_test1.isWalkableNode({static_cast<std::size_t>(-1), 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, static_cast<std::size_t>(-1)}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, 5}));
    EXPECT_FALSE(graph_test1.isWalkableNode({5, 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({5, 5}));
    EXPECT_FALSE(graph_test1.isWalkableNode({0, 6}));
    EXPECT_FALSE(graph_test1.isWalkableNode({6, 0}));
    EXPECT_FALSE(graph_test1.isWalkableNode({6, 6}));

    EXPECT_TRUE(graph_test1.isBarrier({static_cast<std::size_t>(-1), 0}));
    EXPECT_TRUE(graph_test1.isBarrier({0, static_cast<std::size_t>(-1)}));
    EXPECT_TRUE(graph_test1.isBarrier({0, 5}));
    EXPECT_TRUE(graph_test1.isBarrier({5, 0}));
    EXPECT_TRUE(graph_test1.isBarrier({5, 5}));
    EXPECT_TRUE(graph_test1.isBarrier({0, 6}));
    EXPECT_TRUE(graph_test1.isBarrier({6, 0}));
    EXPECT_TRUE(graph_test1.isBarrier({6, 6}));
}

TEST(GridGraphTest, WalkableNeigGridGraphTest)
{
    std::vector test1{
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, false, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1, graph::AllSouroundingNeigbourCalculator{}};

    auto neigs1 = graph_test1.getWalkableNeigbours({2, 2});
    ASSERT_EQ(neigs1.size(), 8);
    EXPECT_NE(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{1, 1}), std::cend(neigs1));
    EXPECT_NE(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{1, 2}), std::cend(neigs1));
    EXPECT_NE(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{1, 3}), std::cend(neigs1));
    EXPECT_NE(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{2, 1}), std::cend(neigs1));
    EXPECT_NE(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{2, 3}), std::cend(neigs1));
    EXPECT_NE(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{3, 1}), std::cend(neigs1));
    EXPECT_NE(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{3, 2}), std::cend(neigs1));
    EXPECT_NE(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{3, 3}), std::cend(neigs1));

    EXPECT_EQ(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{2, 2}), std::cend(neigs1));


    auto neigs2 = graph_test1.getWalkableNeigbours({0, 0});
    ASSERT_EQ(neigs2.size(), 3);

    EXPECT_NE(std::find(std::cbegin(neigs2), std::cend(neigs2), Node{1, 0}), std::cend(neigs2));
    EXPECT_NE(std::find(std::cbegin(neigs2), std::cend(neigs2), Node{0, 1}), std::cend(neigs2));
    EXPECT_NE(std::find(std::cbegin(neigs2), std::cend(neigs2), Node{1, 1}), std::cend(neigs2));


    std::vector test2{
        std::vector{true, true, true, true, true},
        std::vector{true, false, true, true, true},
        std::vector{true, true, false, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test2{test2, graph::AllSouroundingNeigbourCalculator{}};

    auto neigs3 = graph_test2.getWalkableNeigbours({0, 0});
    ASSERT_EQ(neigs3.size(), 2);

    EXPECT_NE(std::find(std::cbegin(neigs3), std::cend(neigs3), Node{1, 0}), std::cend(neigs3));
    EXPECT_NE(std::find(std::cbegin(neigs3), std::cend(neigs3), Node{0, 1}), std::cend(neigs3));
}

TEST(GridGraphTest, WalkableManhattanNeigGridGraphTest)
{
    std::vector test1{
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, false, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1, graph::ManhattanNeigbourCalculator{}};

    auto neigs1 = graph_test1.getWalkableNeigbours({2, 2});
    ASSERT_EQ(neigs1.size(), 4);
    EXPECT_NE(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{1, 2}), std::cend(neigs1));
    EXPECT_NE(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{2, 1}), std::cend(neigs1));
    EXPECT_NE(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{2, 3}), std::cend(neigs1));
    EXPECT_NE(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{3, 2}), std::cend(neigs1));

    EXPECT_EQ(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{1, 1}), std::cend(neigs1));
    EXPECT_EQ(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{2, 2}), std::cend(neigs1));
    EXPECT_EQ(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{1, 3}), std::cend(neigs1));
    EXPECT_EQ(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{3, 3}), std::cend(neigs1));
    EXPECT_EQ(std::find(std::cbegin(neigs1), std::cend(neigs1), Node{3, 1}), std::cend(neigs1));


    auto neigs2 = graph_test1.getWalkableNeigbours({0, 0});
    ASSERT_EQ(neigs2.size(), 2);

    EXPECT_NE(std::find(std::cbegin(neigs2), std::cend(neigs2), Node{1, 0}), std::cend(neigs2));
    EXPECT_NE(std::find(std::cbegin(neigs2), std::cend(neigs2), Node{0, 1}), std::cend(neigs2));

    EXPECT_EQ(std::find(std::cbegin(neigs2), std::cend(neigs2), Node{1, 1}), std::cend(neigs2));


    std::vector test2{
        std::vector{true, true, true, true, true},
        std::vector{false, false, true, true, true},
        std::vector{true, true, false, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test2{test2, graph::ManhattanNeigbourCalculator{}};

    auto neigs3 = graph_test2.getWalkableNeigbours({0, 0});
    ASSERT_EQ(neigs3.size(), 1);

    EXPECT_NE(std::find(std::cbegin(neigs3), std::cend(neigs3), Node{0, 1}), std::cend(neigs3));
    EXPECT_EQ(std::find(std::cbegin(neigs3), std::cend(neigs3), Node{1, 0}), std::cend(neigs3));
}

TEST(GridGraphTest, GridGraphITeratorTest)
{
    std::vector test1{
        std::vector{true, true, false, false, false},
        std::vector{true, true, true, true, true},
        std::vector{true, false, false, true, false},
        std::vector{true, false, false, true, true},
        std::vector{true, true, false, true, false}};

    GridGraph graph_test1{test1, graph::ManhattanNeigbourCalculator{}};

    std::vector<Node> nodes;
    std::copy(std::begin(graph_test1),
              std::end(graph_test1),
              std::back_inserter(nodes));

    for(auto n : nodes) {
        EXPECT_TRUE(graph_test1.isWalkableNode(n));
    }

    EXPECT_EQ(nodes.size(), 15);

	
    std::vector test2{
        std::vector{false, true, false, false, false},
        std::vector{true, true, true, true, true},
        std::vector{true, false, false, true, false},
        std::vector{true, false, false, true, true},
        std::vector{true, true, false, true, true}};

    GridGraph graph_test2{test2, graph::ManhattanNeigbourCalculator{}};

    std::vector<Node> nodes2;
    std::copy(std::begin(graph_test2),
              std::end(graph_test2),
              std::back_inserter(nodes2));

    for(auto n : nodes2) {
        EXPECT_TRUE(graph_test2.isWalkableNode(n));
    }

    EXPECT_EQ(nodes.size(), 15);
}


