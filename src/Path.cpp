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


Path::Path(std::vector<graph::Node> path)
    : path_(std::move(path)) {}


auto Path::pushBack(graph::Node node) -> void
{
    path_.emplace_back(std::move(node));
}
auto Path::pushFront(graph::Node node) -> void
{
    path_.emplace(std::begin(path_),
                  std::move(node));
}

auto Path::getLength() const -> std::size_t
{
    //-1 because when we have source-target we need - 1 to calculate the number
    // of edges between them
    return path_.size() - 1;
}

auto Path::getSource() const -> const graph::Node&
{
    return path_.front();
}

auto Path::getSource() -> graph::Node&
{
    return path_.front();
}

auto Path::getTarget() const -> const graph::Node&
{
    return path_.back();
}

auto Path::getTarget() -> graph::Node&
{
    return path_.back();
}

auto Path::getNodes() const
    -> const std::vector<graph::Node>&
{
    return path_;
}
auto Path::getNodes()
    -> std::vector<graph::Node>&
{
    return path_;
}


auto pathfinding::operator<<(std::ostream& os, const Path& p)
    -> std::ostream&
{
    return os << fmt::format("{}", p.path_);
}


auto pathfinding::findCommonNodes(const std::vector<Path>& paths)
    -> std::vector<Node>
{
    std::vector<std::vector<Node>> raw_paths;
	raw_paths.resize(paths.size());

    std::transform(std::execution::par_unseq,
                   std::cbegin(paths),
                   std::cend(paths),
				   std::begin(raw_paths),
                   [](const auto& path) {
                       auto nodes = path.getNodes();
                       std::sort(std::begin(nodes),
                                 std::end(nodes));
                       return nodes;
                   });

    return std::reduce(std::execution::par_unseq,
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
