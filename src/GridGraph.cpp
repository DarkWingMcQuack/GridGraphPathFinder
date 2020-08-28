#include <GridGraph.hpp>
#include <algorithm>
#include <fmt/core.h>
#include <fstream>
#include <vector>

using graph::GridGraph;
using graph::Node;

GridGraph::GridGraph(std::vector<std::vector<bool>> grid)
    : height(grid.size()),
      width(grid[0].size())
{
    auto total_size = grid.size() * grid[0].size();
    grid_.reserve(total_size);

    for(const auto& sub_grid : grid) {
        grid_.insert(std::end(grid_),
                     std::begin(sub_grid),
                     std::end(sub_grid));
    }
}

auto GridGraph::isBarrier(const Node& n) const
    -> bool
{
    return !isWalkableNode(n);
}

auto GridGraph::isWalkableNode(const Node& n) const
    -> bool
{
    auto row = n.row;
    auto column = n.column;

    if(row >= height || row < 0) {
        return false;
    }

    if(column >= width || column < 0) {
        return false;
    }

    auto index = n.row * width + n.column;
    return grid_[index];
}

auto GridGraph::getWalkableNeigbours(const Node& n) const
    -> std::vector<Node>
{
    std::array raw_neigs{
        Node{n.row, n.column + 1},
        Node{n.row, n.column - 1},
        Node{n.row - 1, n.column},
        Node{n.row + 1, n.column},
        Node{n.row + 1, n.column + 1},
        Node{n.row + 1, n.column - 1},
        Node{n.row - 1, n.column - 1},
        Node{n.row - 1, n.column + 1},
    };

    std::vector<Node> nodes;
    nodes.reserve(8);

    std::copy_if(std::cbegin(raw_neigs),
                 std::cend(raw_neigs),
                 std::back_inserter(nodes),
                 [&](const auto& n) {
                     return isWalkableNode(n);
                 });

    return nodes;
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

        std::vector<std::vector<bool>> grid;
        grid.reserve(height);

        std::string line;
        while(std::getline(graph_file, line)) {

            if(line.size() != width && !line.empty()) {
                fmt::print("line has uncommon width of {}\n", line.size());
                return std::nullopt;
            }

            std::vector<bool> grid_line;
            grid_line.reserve(width);

            std::transform(std::cbegin(line),
                           std::cend(line),
                           std::back_inserter(grid_line),
                           [](char c) {
                               return c == '.';
                           });

            if(grid_line.size() == width) {
                grid.emplace_back(std::move(grid_line));
            }
        }

        return GridGraph{std::move(grid)};

    } catch(...) {
        return std::nullopt;
    }
}
