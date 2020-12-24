#pragma once

#include <fmt/core.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <map>
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

    SelectionLookup(const graph::GridGraph& graph,
                    std::vector<NodeSelection> selections,
                    std::vector<std::vector<std::size_t>> left_selections,
                    std::vector<std::vector<std::size_t>> right_selections);


    [[nodiscard]] auto getOneCommonSelection(const graph::Node& first,
                                             const graph::Node& second) const noexcept
        -> std::optional<utils::CRef<NodeSelection>>;

    [[nodiscard]] auto getAllCommonSelection(const graph::Node& first,
                                             const graph::Node& second) const noexcept
        -> utils::CRefVec<NodeSelection>;

    [[nodiscard]] auto getSizeDistributionLeft() const noexcept
        -> std::map<std::size_t, std::size_t>;

    [[nodiscard]] auto getSizeDistributionRight() const noexcept
        -> std::map<std::size_t, std::size_t>;

    [[nodiscard]] auto getSizeDistributionTotal() const noexcept
        -> std::map<std::size_t, std::size_t>;

    [[nodiscard]] auto sanityCheck() const noexcept
        -> bool;

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
    friend class SelectionLookupOptimizer;
};

} // namespace selection
