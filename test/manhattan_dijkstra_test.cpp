#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <graph/GridGraph.hpp>
#include <pathfinding/Dijkstra.hpp>

#include <gtest/gtest.h>

using graph::GridGraph;
using pathfinding::Dijkstra;


TEST(ManhattanDijkstraTest, DijkstraWithoutBarriersTest)
{
    std::vector test1{
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1, graph::ManhattanNeigbourCalculator{}};

    ASSERT_EQ(graph_test1.getWidth(), 5);
    ASSERT_EQ(graph_test1.getHeight(), 5);

    Dijkstra d{graph_test1};

    auto paths = d.findAllRoutes({0, 0}, {0, 4});

    for(auto& p : paths) {
        EXPECT_EQ(p.getLength(), 4);
    }

    EXPECT_EQ(paths.size(), 1);
}

TEST(ManhattanDijkstraTest, DijkstraSingleWithoutBarriersTest)
{
    std::vector test1{
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1, graph::ManhattanNeigbourCalculator{}};

    ASSERT_EQ(graph_test1.getWidth(), 5);
    ASSERT_EQ(graph_test1.getHeight(), 5);

    Dijkstra d{graph_test1};

    auto path_opt = d.findRoute({0, 0}, {0, 4});

    ASSERT_TRUE((bool)path_opt);

    EXPECT_EQ(path_opt.value().getLength(), 4);

}

TEST(ManhattanDijkstraTest, DijkstraWithBarrierTest)
{
    std::vector test1{
        std::vector{true, true, false, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1, graph::ManhattanNeigbourCalculator{}};

    ASSERT_EQ(graph_test1.getWidth(), 5);
    ASSERT_EQ(graph_test1.getHeight(), 5);

    Dijkstra d{graph_test1};

    auto paths = d.findAllRoutes({0, 0}, {0, 4});

    for(auto& p : paths) {
        EXPECT_EQ(p.getLength(), 6);
    }

    EXPECT_EQ(paths.size(), 4);
}

TEST(ManhattanDijkstraTest, DijkstraSingleRouteWithBarrierTest)
{
    std::vector test1{
        std::vector{true, true, false, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1, graph::ManhattanNeigbourCalculator{}};

    ASSERT_EQ(graph_test1.getWidth(), 5);
    ASSERT_EQ(graph_test1.getHeight(), 5);

    Dijkstra d{graph_test1};

    auto path_opt = d.findRoute({0, 0}, {0, 4});

    ASSERT_TRUE((bool)path_opt);

    EXPECT_EQ(path_opt.value().getLength(), 6);
}

TEST(ManhattanDijkstraTest, DijkstraWithBarrierDistanceTest)
{
    std::vector test1{
        std::vector{true, true, false, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1, graph::ManhattanNeigbourCalculator{}};

    ASSERT_EQ(graph_test1.getWidth(), 5);
    ASSERT_EQ(graph_test1.getHeight(), 5);

    Dijkstra d{graph_test1};

    auto dist = d.findDistance({0, 0}, {0, 4});
    EXPECT_EQ(dist, 6);

    dist = d.findDistance({0, 4}, {0, 0});
    EXPECT_EQ(dist, 6);
}
