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

namespace {

auto findClipValues(const std::vector<std::vector<bool>>& grid)
{
    std::size_t clipped_upper_height = 0;
    for(std::size_t i{0}; i < grid.size(); i++) {
        if(std::none_of(std::begin(grid[i]), std::end(grid[i]), [](bool x) { return x; })) {
            clipped_upper_height++;
        } else {
            break;
        }
    }

    std::size_t clipped_lower_height = 0;
    for(std::size_t i{grid.size()}; i > 0; i--) {
        if(std::none_of(std::begin(grid[i - 1]),
                        std::end(grid[i - 1]),
                        [](bool x) { return x; })) {
            clipped_lower_height++;
        } else {
            break;
        }
    }

    std::size_t clipped_upper_width = 0;
    for(std::size_t i{0}; i < grid[0].size(); i++) {
        for(std::size_t j{0}; j < grid.size(); j++) {
            if(grid[j][i]) {
                goto after_for;
            }
        }

        clipped_upper_width++;
    }
after_for:

    std::size_t clipped_lower_width = 0;
    for(std::size_t i{grid[0].size()}; i > 0; i--) {
        for(std::size_t j{0}; j < grid.size(); j++) {
            if(grid[j][i - 1]) {
                return std::tuple{clipped_upper_height,
                                  clipped_upper_width,
                                  clipped_lower_height,
                                  clipped_lower_width};
            }
        }
        clipped_lower_width++;
    }

    return std::tuple{clipped_upper_height,
                      clipped_upper_width,
                      clipped_lower_height,
                      clipped_lower_width};
}

auto clipGrid(std::vector<std::vector<bool>> grid,
              std::size_t clip_upper_height,
              std::size_t clip_upper_width,
              std::size_t clip_lower_height,
              std::size_t clip_lower_width)
{
    grid.erase(std::begin(grid),
               std::begin(grid) + clip_upper_height);

    if(grid.empty()) {
        return grid;
    }

    grid.resize(grid.size() - clip_lower_height);

    for(auto& row : grid) {
        row.erase(std::begin(row),
                  std::begin(row) + clip_upper_width);
        row.resize(row.size() - clip_lower_width);
    }

    return grid;
}

} // namespace


GridGraph::GridGraph(std::vector<std::vector<bool>> grid,
                     NeigbourCalculator neigbour_calculator) noexcept
    : neigbour_calculator_(neigbour_calculator)
{
    auto [clipped_height,
          clipped_width,
          clipped_lower_height,
          clipped_lower_width] = findClipValues(grid);

    clipped_height_ = clipped_height;
    clipped_width_ = clipped_width;

    grid = clipGrid(std::move(grid),
                    clipped_height_,
                    clipped_width_,
                    clipped_lower_height,
                    clipped_lower_width);

    const auto total_size = grid.size() * grid[0].size();
    grid_.reserve(total_size);

    for(const auto& sub_grid : grid) {
        grid_.insert(std::end(grid_),
                     std::begin(sub_grid),
                     std::end(sub_grid));
    }
    height_ = grid.size();
    width_ = [&] {
        if(grid.empty()) {
            return 0uL;
        }
        return grid[0].size();
    }();
}

auto GridGraph::isBarrier(Node n) const noexcept
    -> bool
{
    return !isWalkableNode(n);
}


auto GridGraph::isWalkableNode(Node n) const noexcept
    -> bool
{
    const auto row = n.row;
    const auto column = n.column;

    if(row >= height_ || row < 0) {
        return false;
    }

    if(column >= width_ || column < 0) {
        return false;
    }

    const auto index = n.row * width_ + n.column;
    return grid_[index];
}

auto GridGraph::getAllCellsContaining(Node node) const noexcept
    -> std::vector<graph::GridCell>
{
    std::vector<graph::GridCell> result{wrapGraphInCell()};

    while(result.back().size() != 1) {

        auto splitted = result.back().split();
        for(auto grid : splitted) {
            if(grid.isInCell(node)) {
                result.emplace_back(grid);
                break;
            }
        }
    }

    return result;
}

auto GridGraph::getAllParrentCells(GridCell cell) const noexcept
    -> std::vector<GridCell>
{
    std::vector<graph::GridCell> result{wrapGraphInCell()};

    while(result.back().size() > cell.size()) {

        auto splitted = result.back().split();
        for(auto grid : splitted) {
            if(grid.isSuperSetOf(cell)) {
                result.emplace_back(grid);
                break;
            }
        }
    }

    return result;
}

auto GridGraph::getAllPossibleSeparationCells(GridCell left, GridCell right) const noexcept
    -> std::pair<std::vector<GridCell>, std::vector<GridCell>>
{
    auto left_candidates = getAllParrentCells(left);
    auto right_candidates = getAllParrentCells(right);

    left_candidates.erase(
        std::remove_if(
            std::begin(left_candidates),
            std::end(left_candidates),
            [&](auto candidate) {
                return candidate.isSuperSetOf(right);
            }),
        std::end(left_candidates));

    right_candidates.erase(
        std::remove_if(
            std::begin(right_candidates),
            std::end(right_candidates),
            [&](auto candidate) {
                return candidate.isSuperSetOf(left);
            }),
        std::end(right_candidates));

    return std::pair{std::move(left_candidates),
                     std::move(right_candidates)};
}

auto GridGraph::countWalkableNodes() const noexcept
    -> std::size_t
{
    return std::accumulate(std::begin(*this),
                           std::end(*this),
                           0,
                           [](auto init, auto /*node*/) {
                               return init + 1;
                           });
}

auto GridGraph::getWalkableNeigbours(Node n) const noexcept
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
    std::uniform_int_distribution<> width_dis(0, width_ - 1 - cell_size);
    std::uniform_int_distribution<> heigth_dis(0, height_ - 1 - cell_size);

    graph::GridCorner top_left{heigth_dis(gen), width_dis(gen)};
    graph::GridCorner bottom_right{top_left.getRow() + cell_size, top_left.getColumn() + cell_size};

    return graph::GridCell{
        top_left,
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

auto GridGraph::areNeighbours(Node first, Node second) const noexcept
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
    auto right_border = static_cast<std::int64_t>(width_ - 1);
    auto bottom_border = static_cast<std::int64_t>(height_ - 1);

    graph::GridCorner top_left{0, 0};
    graph::GridCorner bottom_right{bottom_border, right_border};

    return graph::GridCell{top_left,
                           bottom_right};
}


auto GridGraph::getRandomWalkableNode() const noexcept
    -> Node
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> width_dis(0, width_ - 1);
    std::uniform_int_distribution<std::size_t> heigth_dis(0, height_ - 1);

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
    return GridGraphIterator{*this, width_ * height_};
}

auto GridGraph::getHeight() const noexcept
    -> std::size_t
{
    return height_;
}

auto GridGraph::getWidth() const noexcept
    -> std::size_t
{
    return width_;
}

auto GridGraph::toClipped(Node n) const noexcept
    -> Node
{
    return Node{n.row - clipped_height_,
                n.column - clipped_width_};
}

auto GridGraph::unclip(Node n) const noexcept
    -> Node
{
    return Node{n.row + clipped_height_,
                n.column + clipped_width_};
}


auto GridGraph::toClipped(GridCorner g) const noexcept
    -> GridCorner
{
    return GridCorner{static_cast<int64_t>(g.getRow() - clipped_height_),
                      static_cast<int64_t>(g.getColumn() - clipped_width_)};
}

auto GridGraph::unclip(GridCorner g) const noexcept
    -> GridCorner
{
    return GridCorner{static_cast<int64_t>(g.getRow() + clipped_height_),
                      static_cast<int64_t>(g.getColumn() + clipped_width_)};
}

auto GridGraph::toClipped(GridCell g) const noexcept
    -> GridCell
{
    return GridCell{
        toClipped(g.getTopLeft()),
        toClipped(g.getBottomRight()),
    };
}

auto GridGraph::unclip(GridCell g) const noexcept
    -> GridCell
{
    return GridCell{
        unclip(g.getTopLeft()),
        unclip(g.getBottomRight()),
    };
}

auto GridGraph::unclip(separation::TrivialSeparation g) const noexcept
    -> separation::TrivialSeparation
{
    auto left = g.getFirstCluster();
    auto right = g.getSecondCluster();

    return separation::TrivialSeparation{unclip(left),
                                         unclip(right)};
}

auto GridGraph::unclip(separation::ComplexSeparation g) const noexcept
    -> separation::ComplexSeparation
{
    auto left = g.getFirstCluster();
    auto right = g.getSecondCluster();
    auto left_center = g.getFirstClusterCenter();
    auto right_center = g.getSecondClusterCenter();

    return separation::ComplexSeparation{unclip(left),
                                         unclip(right),
                                         unclip(left_center),
                                         unclip(right_center),
                                         g.getCenterDistance()};
}

auto GridGraph::unclip(separation::Separation g) const noexcept
    -> separation::Separation
{
    return std::visit(
        [&](auto sep) -> separation::Separation {
            return unclip(sep);
        },
        g);
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

        return GridGraph{std::move(grid), neigbour_calc};

    } catch(...) {
        return std::nullopt;
    }
}
