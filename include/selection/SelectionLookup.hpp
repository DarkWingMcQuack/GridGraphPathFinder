#pragma once

#include <fmt/core.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <pathfinding/Distance.hpp>
#include <queue>
#include <random>
#include <selection/NodeSelection.hpp>
#include <selection/NodeSelectionCalculator.hpp>
#include <unordered_set>
#include <vector>

namespace selection {

class SelectionLookup
{
public:
    SelectionLookup(const graph::GridGraph& graph,
                    std::vector<NodeSelection> selections);


    [[nodiscard]] auto getOneCommonSelection(const graph::Node& first,
                                             const graph::Node& second) const noexcept
        -> std::optional<utils::CRef<NodeSelection>>;

    [[nodiscard]] auto getAllCommonSelection(const graph::Node& first,
                                             const graph::Node& second) const noexcept
        -> utils::RefVec<NodeSelection>;

private:
    [[nodiscard]] auto getOneCommonSelection(
        const std::vector<std::size_t>& first,
        const std::vector<std::size_t>& second) const noexcept
        -> std::optional<std::size_t>;

    [[nodiscard]] auto getAllCommonSelection(
        const std::vector<std::size_t>& first,
        const std::vector<std::size_t>& second) const noexcept
        -> std::vector<std::size_t>;


private:
    const graph::GridGraph& graph_;
    std::vector<NodeSelection> selections_;

    std::vector<std::vector<std::size_t>> left_selections_;
    std::vector<std::vector<std::size_t>> right_selections_;

    friend class SelectionBucketCreator;
};

} // namespace selection
