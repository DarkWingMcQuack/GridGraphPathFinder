#include <graph/Node.hpp>
#include <graph/NormalGraph.hpp>
#include <nonstd/span.hpp>
#include <pathfinding/Distance.hpp>
#include <utils/Utils.hpp>

using graph::NormalGraph;

NormalGraph::NormalGraph(std::vector<std::vector<std::pair<NodeId, Distance>>>&& adj_list)
    : offset_(adj_list.size() + 1, 0)
{
    for(auto i = 0; i < adj_list.size(); i++) {
        auto neigs = std::move(adj_list[i]);
        neigbours_.insert(std::end(neigbours_),
                          std::begin(neigs),
                          std::end(neigs));

        offset_[i + 1] = neigbours_.size();
    }

    neigbours_.emplace_back(std::numeric_limits<NodeId>::max(), UNREACHABLE);
}

auto NormalGraph::getNeigboursOf(NodeId node) const noexcept
    -> nonstd::span<const std::pair<NodeId, Distance>>
{
    const auto start_offset = offset_[node];
    const auto end_offset = offset_[node + 1];
    const auto* start = &neigbours_[start_offset];
    const auto* end = &neigbours_[end_offset];

    return nonstd::span{start, end};
}

auto NormalGraph::size() const noexcept
    -> std::size_t
{
    return offset_.size() - 1;
}
