#pragma once

#include <graph/Node.hpp>
#include <nonstd/span.hpp>
#include <pathfinding/Distance.hpp>

namespace graph {

class NormalGraph
{
public:
    NormalGraph(std::vector<std::vector<std::pair<NodeId, Distance>>>&& adj_list);

    auto getNeigboursOf(NodeId node) const noexcept
        -> nonstd::span<const std::pair<NodeId, Distance>>;

    auto size() const noexcept
        -> std::size_t;

private:
    std::vector<std::pair<NodeId, Distance>> neigbours_;
    std::vector<size_t> offset_;
};

auto parseFMIFile(std::string_view path) noexcept
    -> std::optional<NormalGraph>;

} // namespace graph
