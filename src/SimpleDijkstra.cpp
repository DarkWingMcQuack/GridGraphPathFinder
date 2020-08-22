#include <GridGraph.hpp>
#include <SimpleDijkstra.hpp>
#include <functional>
#include <optional>
#include <queue>
#include <string_view>
#include <vector>

using graph::Node;
using graph::GridGraph;
using pathfinding::SimpleDijkstra;

SimpleDijkstra::SimpleDijkstra(const graph::GridGraph& graph)
    : graph_(graph),
      distances_(graph.width * graph.height, -1) {}


auto SimpleDijkstra::findRoutes(const graph::Node& source, const graph::Node& target)
    -> std::vector<Path>
{
    auto touched = computeDistances(source, target);
    auto path = extractShortestPaths(source, target);
    resetDistances(touched);

    return path;
}

auto SimpleDijkstra::getIndex(const graph::Node& n) const
    -> std::optional<std::size_t>
{
    auto row = n.row;
    auto column = n.column;

    if(row >= graph_.get().height || row < 0) {
        return std::nullopt;
    }

    if(column >= graph_.get().width || column < 0) {
        return std::nullopt;
    }

    return n.row * graph_.get().width + n.column;
}

auto SimpleDijkstra::getDistanceTo(const graph::Node& n) const
    -> std::optional<std::int64_t>
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        return distances_[index_opt.value()];
    }

    return std::nullopt;
}


auto SimpleDijkstra::setDistanceTo(const graph::Node& n, std::int64_t distance)
    -> void
{
    auto index_opt = getIndex(n);

    if(index_opt) {
        distances_[index_opt.value()] = distance;
    }
}

auto SimpleDijkstra::resetDistances(const std::vector<graph::Node>& touched)
    -> void
{
    for(const auto& n : touched) {
        setDistanceTo(n, -1);
    }
}

auto SimpleDijkstra::extractShortestPaths(const graph::Node& source, const graph::Node& target) const
    -> std::vector<Path>
{
    //check if a path exists
    if(auto dist_opt = getDistanceTo(target);
       !dist_opt || dist_opt.value() == -1) {
        return {};
    }

    Path initial{std::vector{target}};
    std::vector<Path> unfinished_paths{std::move(initial)};
    std::vector<Path> complete_paths;

    while(!unfinished_paths.empty()) {
        auto path = std::move(unfinished_paths.back());
        unfinished_paths.pop_back();

        const auto& last_inserted = path.getSource();
        auto neigbours = graph_.get().getWalkableNeigbours(last_inserted);

        auto smallest_distance_opt = findSmallestDistance(neigbours);
        if(!smallest_distance_opt) {
            continue;
        }

        auto smallest_distance = smallest_distance_opt.value();

        for(const auto& neig : neigbours) {
            auto dist = getDistanceTo(neig).value();

            if(dist != smallest_distance) {
                continue;
            }

            auto path_copy = path;
            path_copy.pushFront(neig);
            if(neig == source) {
                complete_paths.emplace_back(std::move(path_copy));
            } else {
                unfinished_paths.emplace_back(std::move(path_copy));
            }
        }
    }

    return complete_paths;
}


auto SimpleDijkstra::getWalkableNeigboursOf(const graph::Node& n) const
    -> std::vector<graph::Node>
{
    return graph_.get().getWalkableNeigbours(n);
}


namespace {
constexpr auto dijkstra_comperator = [](const std::pair<Node, std::int64_t>& lhs,
                                        const std::pair<Node, std::int64_t>& rhs) {
    return lhs.second < rhs.second;
};

using DijkstraQueue = std::priority_queue<std::pair<Node, std::int64_t>,
                                          std::vector<std::pair<Node, std::int64_t>>,
                                          decltype(dijkstra_comperator)>;
} // namespace

auto SimpleDijkstra::computeDistances(const graph::Node& source, const graph::Node& target)
    -> std::vector<graph::Node>
{
    DijkstraQueue queue(dijkstra_comperator);
    queue.emplace(source, 0l);

    std::vector<Node> touched;

    while(!queue.empty()) {
        auto [current_node, current_dist] = std::move(queue.top());
        queue.pop();

        if(current_node == target) {
            return touched;
        }

        auto neigbours = graph_.get().getWalkableNeigbours(current_node);

        for(auto&& neig : neigbours) {
            touched.emplace_back(neig);

            auto neig_dist = getDistanceTo(neig).value();

            if(neig_dist == -1 || neig_dist > current_dist + 1) {
                setDistanceTo(neig, current_dist + 1);
                queue.emplace(neig, current_dist + 1);
            }
        }
    }

    return touched;
}
