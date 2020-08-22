#include <GridGraph.hpp>
#include <Path.hpp>
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
    return path_.size();
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
