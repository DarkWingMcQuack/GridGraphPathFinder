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
    [[nodiscard]] auto getSelections(const graph::Node& first,
                                     const graph::Node& second) const noexcept
        -> std::pair<
            utils::CRef<utils::RefVec<NodeSelection>>,
            utils::CRef<utils::RefVec<NodeSelection>>>;


    [[nodiscard]] auto getOneCommonSelection(
        const utils::RefVec<NodeSelection>& first,
        const utils::RefVec<NodeSelection>& second) const noexcept
        -> std::optional<utils::CRef<NodeSelection>>;

    [[nodiscard]] auto getAllCommonSelection(
        const utils::RefVec<NodeSelection>& first,
        const utils::RefVec<NodeSelection>& second) const noexcept
        -> utils::RefVec<NodeSelection>;


    [[nodiscard]] auto getNodeIndex(const graph::Node& n) const noexcept
        -> std::size_t;

private:
    const graph::GridGraph& graph_;
    std::vector<NodeSelection> selections_;
    std::vector<utils::RefVec<NodeSelection>> selection_lookup_;
};

} // namespace selection
