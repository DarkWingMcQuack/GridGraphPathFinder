#include <GridGraph.hpp>
#include <Path.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <functional>
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


auto pathfinding::operator<<(std::ostream& os, const Path& p)
    -> std::ostream&
{
    return os << fmt::format("{}", p.path_);
}
