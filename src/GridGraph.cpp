#include <GridGraph.hpp>
#include <algorithm>
#include <fmt/core.h>
#include <fstream>
#include <random>
#include <vector>

using graph::GridGraph;
using graph::Node;

GridGraph::GridGraph(std::vector<std::vector<bool>> grid) noexcept
    : height(grid.size()),
      width(grid[0].size())
{
    const auto total_size = grid.size() * grid[0].size();
    grid_.reserve(total_size);

    for(const auto& sub_grid : grid) {
        grid_.insert(std::end(grid_),
                     std::begin(sub_grid),
                     std::end(sub_grid));
    }
}

auto GridGraph::isBarrier(const Node& n) const noexcept
    -> bool
{
    return !isWalkableNode(n);
}

auto GridGraph::isWalkableNode(const Node& n) const noexcept
    -> bool
{
    const auto row = n.row;
    const auto column = n.column;

    if(row >= height || row < 0) {
        return false;
    }

    if(column >= width || column < 0) {
        return false;
    }

    const auto index = n.row * width + n.column;
    return grid_[index];
}

auto GridGraph::getWalkableNeigbours(const Node& n) const noexcept
    -> std::vector<Node>
{
    const std::array raw_neigs{
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


auto GridGraph::getWalkableManhattanNeigbours(const Node& n) const noexcept
    -> std::vector<Node>
{
    const std::array raw_neigs{
        Node{n.row, n.column + 1},
        Node{n.row, n.column - 1},
        Node{n.row - 1, n.column},
        Node{n.row + 1, n.column},
    };

    std::vector<Node> nodes;
    nodes.reserve(4);

    std::copy_if(std::cbegin(raw_neigs),
                 std::cend(raw_neigs),
                 std::back_inserter(nodes),
                 [&](const auto& n) {
                     return isWalkableNode(n);
                 });

    return nodes;
}

auto GridGraph::getAllWalkableNodesOfCell(const grid::GridCell& cell) const noexcept
    -> std::vector<Node>
{
    std::vector<Node> nodes;
    nodes.reserve(cell.getWidth() * cell.getWidth());

    std::copy_if(std::cbegin(cell),
                 std::cend(cell),
                 std::back_inserter(nodes),
                 [&](const auto& node) {
                     return isWalkableNode(node);
                 });

    return nodes;
}


auto GridGraph::generateRandomCellOfSize(std::int64_t cell_size) const noexcept
    -> grid::GridCell
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> width_dis(0, width - 1);
    std::uniform_int_distribution<> heigth_dis(0, height - 1);

    grid::GridCorner top_left{heigth_dis(gen), width_dis(gen)};
    grid::GridCorner top_right{top_left.getRow(), top_left.getColumn() + cell_size};
    grid::GridCorner bottom_left{top_left.getRow() + cell_size, top_left.getColumn()};
    grid::GridCorner bottom_right{top_left.getRow() + cell_size, top_left.getColumn() + cell_size};

    return grid::GridCell{
        top_left,
        top_right,
        bottom_left,
        bottom_right};
}

auto graph::parseFileToGridGraph(std::string_view path) noexcept
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
