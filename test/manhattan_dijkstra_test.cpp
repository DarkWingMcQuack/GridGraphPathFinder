#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <graph/GridGraph.hpp>
#include <pathfinding/ManhattanDijkstra.hpp>

#include <gtest/gtest.h>

using graph::GridGraph;
using pathfinding::ManhattanDijkstra;


TEST(ManhattanDijkstraTest, ManhattanDijkstraWithoutBarriersTest)
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

    auto paths = d.findAllRoutes({0, 0}, {0, 4});

    for(auto& p : paths) {
        EXPECT_EQ(p.getLength(), 4);
    }

    EXPECT_EQ(paths.size(), 1);
}

TEST(ManhattanDijkstraTest, ManhattanDijkstraSingleWithoutBarriersTest)
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

    auto path_opt = d.findRoute({0, 0}, {0, 4});

    ASSERT_TRUE((bool)path_opt);

    EXPECT_EQ(path_opt.value().getLength(), 4);

}

TEST(ManhattanDijkstraTest, ManhattanDijkstraWithBarrierTest)
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

    auto paths = d.findAllRoutes({0, 0}, {0, 4});

    for(auto& p : paths) {
        EXPECT_EQ(p.getLength(), 6);
    }

    EXPECT_EQ(paths.size(), 4);
}

TEST(ManhattanDijkstraTest, ManhattanDijkstraSingleRouteWithBarrierTest)
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

    auto path_opt = d.findRoute({0, 0}, {0, 4});

    ASSERT_TRUE((bool)path_opt);

    EXPECT_EQ(path_opt.value().getLength(), 6);
}

TEST(ManhattanDijkstraTest, ManhattanDijkstraWithBarrierDistanceTest)
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

    auto dist = d.findDistance({0, 0}, {0, 4});
    EXPECT_EQ(dist, 6);

    dist = d.findDistance({0, 4}, {0, 0});
    EXPECT_EQ(dist, 6);
}
