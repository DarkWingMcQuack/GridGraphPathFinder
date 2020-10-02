#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <graph/GridGraph.hpp>
#include <pathfinding/Dijkstra.hpp>

#include <gtest/gtest.h>

using graph::GridGraph;
using pathfinding::Dijkstra;


TEST(StraightDijkstraTest, DijkstraWithoutBarriersTest)
{
    std::vector test1{
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true},
        std::vector{true, true, true, true, true}};

    GridGraph graph_test1{test1, graph::AllSouroundingNeigbourCalculator{}};

    ASSERT_EQ(graph_test1.width, 5);
    ASSERT_EQ(graph_test1.height, 5);

    Dijkstra d{graph_test1};

    auto paths = d.findAllRoutes({0, 0}, {0, 4});

    for(auto& p : paths) {
        EXPECT_EQ(p.getLength(), 4);
    }

    EXPECT_EQ(paths.size(), 9);
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

    ASSERT_EQ(graph_test1.width, 5);
    ASSERT_EQ(graph_test1.height, 5);

    Dijkstra d{graph_test1};

    auto paths = d.findAllRoutes({0, 0}, {0, 4});

    for(auto& p : paths) {
        EXPECT_EQ(p.getLength(), 4);
    }

    EXPECT_EQ(paths.size(), 5);
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

    ASSERT_EQ(graph_test1.width, 5);
    ASSERT_EQ(graph_test1.height, 5);

    Dijkstra d{graph_test1};

    auto paths = d.findAllRoutes({0, 0}, {0, 4});

    for(auto& p : paths) {
        EXPECT_EQ(p.getLength(), 8);
    }

    EXPECT_EQ(paths.size(), 4);
}
