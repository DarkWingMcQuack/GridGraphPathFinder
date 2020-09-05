#include <GridGraph.hpp>
#include <Path.hpp>
#include <execution>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <functional>
#include <numeric>
#include <optional>
#include <string_view>
#include <vector>


using graph::Node;
using pathfinding::Path;


Path::Path(std::vector<graph::Node> path) noexcept
    : path_(std::move(path)) {}


auto Path::pushBack(graph::Node node) noexcept -> void
{
    path_.emplace_back(node);
}
auto Path::pushFront(graph::Node node) noexcept -> void
{
    path_.emplace(std::begin(path_),
                  node);
}

auto Path::getLength() const noexcept -> std::size_t
{
    //-1 because when we have source-target we need - 1 to calculate the number
    // of edges between them
    return path_.size() - 1;
}

auto Path::getSource() const noexcept -> const graph::Node&
{
    return path_.front();
}

auto Path::getSource() noexcept -> graph::Node&
{
    return path_.front();
}

auto Path::getTarget() const noexcept -> const graph::Node&
{
    return path_.back();
}

auto Path::getTarget() noexcept -> graph::Node&
{
    return path_.back();
}

auto Path::getNodes() const noexcept
    -> const std::vector<graph::Node>&
{
    return path_;
}
auto Path::getNodes() noexcept
    -> std::vector<graph::Node>&
{
    return path_;
}


auto Path::getNodesIn(const grid::GridCell& cell) const noexcept
    -> std::vector<graph::Node>
{
    std::vector<Node> found;
    std::copy_if(std::cbegin(path_),
                 std::cend(path_),
                 std::back_inserter(found),
                 [&](const auto& node) {
                     return cell.isInCell(node);
                 });

    return found;
}

auto Path::contains(const graph::Node& node) const noexcept
    -> bool
{
    return std::find(std::cbegin(path_),
                     std::cend(path_),
                     node)
        != std::cend(path_);
}


auto pathfinding::operator<<(std::ostream& os, const Path& p) noexcept
    -> std::ostream&
{
    return os << fmt::format("{}", p.path_);
}


auto pathfinding::findCommonNodes(const std::vector<Path>& paths) noexcept
    -> std::vector<Node>
{
    std::vector<std::vector<Node>> raw_paths;
    raw_paths.resize(paths.size());

    std::transform(std::execution::par,
                   std::cbegin(paths),
                   std::cend(paths),
                   std::begin(raw_paths),
                   [](const auto& path) {
                       auto nodes = path.getNodes();
                       std::sort(std::begin(nodes),
                                 std::end(nodes));
                       return nodes;
                   });

    return std::reduce(std::execution::par,
                       std::make_move_iterator(std::begin(raw_paths)),
                       std::make_move_iterator(std::end(raw_paths)),
                       std::vector<Node>{},
                       [](auto&& acc, auto&& next) {
                           std::vector<Node> intersect;

                           std::set_intersection(std::begin(acc),
                                                 std::end(acc),
                                                 std::begin(next),
                                                 std::end(next),
                                                 std::back_inserter(intersect));

                           return intersect;
                       });
}
