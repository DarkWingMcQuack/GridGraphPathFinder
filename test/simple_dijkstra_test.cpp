#include <GridGraph.hpp>
#include <SimpleDijkstra.hpp>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

#include <gtest/gtest.h>

using graph::Node;
using graph::GridGraph;
using pathfinding::SimpleDijkstra;
using pathfinding::Path;


TEST(SimpleDijkstraTest, SimpleDijkstraWithoutBarriersTest)
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

    SimpleDijkstra d{graph_test1};

    auto paths = d.findRoutes({0, 0}, {0, 4});

    for(auto& p : paths) {
	  EXPECT_EQ(p.getLength(), 4);
    }

    EXPECT_EQ(paths.size(), 8);
}
