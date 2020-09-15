#include <algorithm>
#include <fmt/core.h>
#include <fstream>
#include <graph/GridGraph.hpp>
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

auto GridGraph::getAllWalkableNodesOfCell(const graph::GridCell& cell) const noexcept
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
    -> graph::GridCell
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> width_dis(0, width - 1 - cell_size);
    std::uniform_int_distribution<> heigth_dis(0, height - 1 - cell_size);

    graph::GridCorner top_left{heigth_dis(gen), width_dis(gen)};
    graph::GridCorner top_right{top_left.getRow(), top_left.getColumn() + cell_size};
    graph::GridCorner bottom_left{top_left.getRow() + cell_size, top_left.getColumn()};
    graph::GridCorner bottom_right{top_left.getRow() + cell_size, top_left.getColumn() + cell_size};

    return graph::GridCell{
        top_left,
        top_right,
        bottom_left,
        bottom_right};
}


auto GridGraph::hasWalkableNode(const graph::GridCell& cell) const noexcept
    -> bool
{
    return std::any_of(std::begin(cell),
                       std::end(cell),
                       [&](const auto& node) {
                           return isWalkableNode(node);
                       });
}

auto GridGraph::countNumberOfWalkableNodes(const graph::GridCell& cell) const noexcept
    -> std::size_t
{
    return std::count_if(std::begin(cell),
                         std::end(cell),
                         [&](const auto& node) {
                             return isWalkableNode(node);
                         });
}

auto GridGraph::wrapGraphInCell() const noexcept
    -> graph::GridCell
{
    auto right_border = static_cast<std::int64_t>(width - 1);
    auto bottom_border = static_cast<std::int64_t>(height - 1);

    graph::GridCorner top_left{0, 0};
    graph::GridCorner top_right{0, right_border};
    graph::GridCorner bottom_left{bottom_border, 0};
    graph::GridCorner bottom_right{bottom_border, right_border};

    return graph::GridCell{top_left,
                           top_right,
                           bottom_left,
                           bottom_right};
}


auto GridGraph::getRandomWalkableNode() const noexcept
    -> Node
{

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> width_dis(0, width - 1);
    std::uniform_int_distribution<std::size_t> heigth_dis(0, height - 1);

    Node random_node{heigth_dis(gen), width_dis(gen)};

    while(isBarrier(random_node)) {
        random_node = Node{heigth_dis(gen),
                           width_dis(gen)};
    }

    return random_node;
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