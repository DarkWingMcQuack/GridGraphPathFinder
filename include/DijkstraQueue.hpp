#include <Distance.hpp>
#include <Node.hpp>
#include <queue>
#include <string_view>
#include <vector>

namespace pathfinding {

struct DijkstraQueueComparer
{
    auto operator()(const std::pair<graph::Node, graph::Distance>& lhs,
                    const std::pair<graph::Node, graph::Distance>& rhs) const noexcept
        -> bool
    {
        return lhs.second > rhs.second;
    }
};

using DijkstraQueue = std::priority_queue<std::pair<graph::Node, graph::Distance>,
                                          std::vector<std::pair<graph::Node, graph::Distance>>,
                                          DijkstraQueueComparer>;

} // namespace pathfinding
