#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <graph/GridGraph.hpp>
#include <pathfinding/Dijkstra.hpp>

#include <gtest/gtest.h>

using graph::GridGraph;
using pathfinding::GridGraphDijkstra;


TEST(StraightDijkstraTest, DijkstraWithoutBarriersTest)
{
    std::vector test1{
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1, graph::AllSouroundingNeigbourCalculator{}};

    ASSERT_EQ(graph_test1.getWidth(), 5);
    ASSERT_EQ(graph_test1.getHeight(), 5);

    GridGraphDijkstra d{graph_test1};

    auto p = d.findRoute({0, 0}, {0, 4});

    EXPECT_EQ(p.value().getLength(), 4);
}

TEST(StraightDijkstraTest, DijkstraWithOneBarrierTest)
{
    std::vector test1{
        std::vector{true, true, false, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1, graph::AllSouroundingNeigbourCalculator{}};

    ASSERT_EQ(graph_test1.getWidth(), 5);
    ASSERT_EQ(graph_test1.getHeight(), 5);

    GridGraphDijkstra d{graph_test1};

    auto p = d.findRoute({0, 0}, {0, 4});

    EXPECT_EQ(p.value().getLength(), 6);
}

TEST(StraightDijkstraTest, DijkstraWithMoreBarriersTest)
{
    std::vector test1{
        std::vector{true, true, false, true, true},
        std::vector{true, true, false, true, true},
        std::vector{true, true, false, false, true},
        std::vector{true, true, true, false, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1, graph::AllSouroundingNeigbourCalculator{}};

    ASSERT_EQ(graph_test1.getWidth(), 5);
    ASSERT_EQ(graph_test1.getHeight(), 5);

    GridGraphDijkstra d{graph_test1};

    auto paths = d.findRoute({0, 0}, {0, 4});

    EXPECT_EQ(paths.value().getLength(), 12);
}
