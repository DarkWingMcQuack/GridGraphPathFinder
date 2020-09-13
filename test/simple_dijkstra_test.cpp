#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <graph/GridGraph.hpp>
#include <pathfinding/ManhattanDijkstra.hpp>

#include <gtest/gtest.h>

using graph::Node;
using graph::GridGraph;
using pathfinding::ManhattanDijkstra;
using pathfinding::Path;


TEST(StraightManhattanDijkstraTest, ManhattanDijkstraWithoutBarriersTest)
{
    std::vector test1{
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1};

    ASSERT_EQ(graph_test1.width, 5);
    ASSERT_EQ(graph_test1.height, 5);

    ManhattanDijkstra d{graph_test1};

    auto paths = d.findRoutes({0, 0}, {0, 4});

    for(auto& p : paths) {
        EXPECT_EQ(p.getLength(), 4);
    }

    EXPECT_EQ(paths.size(), 9);
}

TEST(StraightManhattanDijkstraTest, ManhattanDijkstraWithOneBarrierTest)
{
    std::vector test1{
        std::vector{true, true, false, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1};

    ASSERT_EQ(graph_test1.width, 5);
    ASSERT_EQ(graph_test1.height, 5);

    ManhattanDijkstra d{graph_test1};

    auto paths = d.findRoutes({0, 0}, {0, 4});

    for(auto& p : paths) {
        EXPECT_EQ(p.getLength(), 4);
    }

    EXPECT_EQ(paths.size(), 5);
}

TEST(StraightManhattanDijkstraTest, ManhattanDijkstraWithMoreBarriersTest)
{
    std::vector test1{
        std::vector{true, true, false, true, true},
        std::vector{true, true, false, true, true},
        std::vector{true, true, false, false, true},
        std::vector{true, true, true, false, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1};

    ASSERT_EQ(graph_test1.width, 5);
    ASSERT_EQ(graph_test1.height, 5);

    ManhattanDijkstra d{graph_test1};

    auto paths = d.findRoutes({0, 0}, {0, 4});

    for(auto& p : paths) {
        EXPECT_EQ(p.getLength(), 8);
    }

    EXPECT_EQ(paths.size(), 4);
}
