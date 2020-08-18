#include <GridGraph.hpp>
#include <algorithm>
#include <fstream>
#include <vector>

using graph::GridGraph;
using graph::Node;

GridGraph::GridGraph(std::vector<std::vector<bool>> grid)
{
    auto total_size = grid.size() * grid[0].size();
    grid_.reserve(total_size);

    for(auto&& sub_grid : std::move(grid)) {
        grid_.insert(std::end(grid_),
                     std::begin(sub_grid),
                     std::end(sub_grid));
    }
}

GridGraph::GridGraph(std::vector<bool> grid)
    : grid_(std::move(grid)) {}

auto GridGraph::isBarrier(const Node& n) const
    -> bool
{
    return !grid_[n.row * n.column];
}

auto GridGraph::isWalkableNode(const Node& n) const
    -> bool
{
    return grid_[n.row * n.column];
}

auto graph::parseFileToGridGraph(std::string_view path)
    -> std::optional<GridGraph>
{
    try {
        std::ifstream graph_file{path.data()};

        int height{0};
        int width{0};
        std::string dummy;

        graph_file >> dummy >> dummy;
        graph_file >> dummy >> height;
        graph_file >> dummy >> width;
        graph_file >> dummy;

        std::vector<std::vector<bool>> grid(height);

        std::string line;
        while(std::getline(graph_file, line)) {
            std::vector<bool> grid_line(width);

            std::transform(std::cbegin(line),
                           std::cend(line),
                           std::back_inserter(grid_line),
                           [](char c) {
                               return c == '.';
                           });

            grid.emplace_back(std::move(grid_line));
        }

        return GridGraph{std::move(grid)};

    } catch(...) {
        return std::nullopt;
    }
}
