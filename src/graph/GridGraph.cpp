#include <algorithm>
#include <fmt/core.h>
#include <fstream>
#include <graph/GridGraph.hpp>
#include <graph/NeigbourCalculator.hpp>
#include <random>
#include <vector>

using graph::GridGraph;
using graph::Node;
using graph::NeigbourCalculator;


GridGraph::GridGraph(std::vector<std::vector<bool>> grid,
                     NeigbourCalculator neigbour_calculator) noexcept
    : height(grid.size()),
      width(grid[0].size()),
      neigbour_calculator_(std::move(neigbour_calculator))
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
    auto neigs = graph::calculateNeigbours(neigbour_calculator_, n);

    neigs.erase(
        std::remove_if(std::begin(neigs),
                       std::end(neigs),
                       [&](auto node) {
                           return !isWalkableNode(node);
                       }),
        std::end(neigs));


    return neigs;
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

auto GridGraph::areNeighbours(const Node& first, const Node& second) const noexcept
    -> bool
{
    return isNeigbourOf(neigbour_calculator_, first, second);
}

auto GridGraph::hasBarrier(const graph::GridCell& cell) const noexcept
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

auto GridGraph::begin() const noexcept
    -> GridGraphIterator
{
    return GridGraphIterator{*this};
}

auto GridGraph::end() const noexcept
    -> GridGraphIterator
{
    return GridGraphIterator{*this, width * height};
}

auto graph::parseFileToGridGraph(std::string_view path,
                                 NeigbourCalculator neigbour_calc) noexcept
    -> std::optional<GridGraph>
{
    try {
        std::ifstream graph_file{path.data()};

        std::size_t height{0};
        std::size_t width{0};
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

        return GridGraph{std::move(grid), std::move(neigbour_calc)};

    } catch(...) {
        return std::nullopt;
    }
}
