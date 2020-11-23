#include <fmt/core.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <pathfinding/Distance.hpp>
#include <queue>
#include <random>
#include <selection/NodeSelection.hpp>
#include <selection/NodeSelectionCalculator.hpp>
#include <selection/SelectionLookup.hpp>
#include <unordered_set>
#include <utils/Utils.hpp>
#include <vector>

using selection::SelectionLookup;
using selection::NodeSelection;

SelectionLookup::SelectionLookup(const graph::GridGraph& graph,
                                 std::vector<NodeSelection> selections)
    : graph_(graph),
      selections_(std::move(selections)),
      selection_lookup_(graph.countWalkableNodes())
{
    for(auto& selection : selections_) {
        for(auto node : selection.getLeftSelection()) {
            auto idx = getNodeIndex(node);
            selection_lookup_[idx].emplace_back(&selection);
        }

        for(auto node : selection.getRightSelection()) {
            auto idx = getNodeIndex(node);
            selection_lookup_[idx].emplace_back(&selection);
        }
    }

    for(auto& selections : selection_lookup_) {
        std::sort(std::begin(selections),
                  std::end(selections));
        fmt::print("number of selections {}\n", selections.size());
    }
}


auto SelectionLookup::getOneCommonSelection(const graph::Node& first,
                                            const graph::Node& second) const noexcept
    -> std::optional<std::reference_wrapper<const NodeSelection>>
{
    auto [first_selections, second_selections] = getSelections(first, second);

    return getOneCommonSelection(first_selections.get(),
                                 second_selections.get());
}

auto SelectionLookup::getAllCommonSelection(const graph::Node& first,
                                            const graph::Node& second) const noexcept
    -> std::vector<NodeSelection*>
{
    auto [first_selections, second_selections] = getSelections(first, second);

    return getAllCommonSelection(first_selections.get(),
                                 second_selections.get());
}


auto SelectionLookup::getSelections(const graph::Node& first,
                                    const graph::Node& second) const noexcept
    -> std::pair<
        std::reference_wrapper<const std::vector<NodeSelection*>>,
        std::reference_wrapper<const std::vector<NodeSelection*>>>
{
    auto first_idx = getNodeIndex(first);
    auto second_idx = getNodeIndex(second);
    auto first_selections = std::cref(selection_lookup_[first_idx]);
    auto second_selections = std::cref(selection_lookup_[second_idx]);

    return std::pair{first_selections, second_selections};
}


auto SelectionLookup::getOneCommonSelection(
    const std::vector<NodeSelection*>& first,
    const std::vector<NodeSelection*>& second) const noexcept
    -> std::optional<std::reference_wrapper<const NodeSelection>>
{
    auto iter1 = std::begin(first);
    auto iter2 = std::begin(second);
    auto iter1_end = std::end(first);
    auto iter2_end = std::end(second);

    while(iter1 != iter1_end && iter2 != iter2_end) {
        if(*iter1 < *iter2) {
            ++iter1;
        } else if(*iter2 < *iter1) {
            ++iter2;
        } else {
            return std::cref(**iter1);
        }
    }
    return std::nullopt;
}

auto SelectionLookup::getNodeIndex(const graph::Node& n) const noexcept
    -> std::size_t
{
    return n.row * graph_.getWidth() + n.column;
}

auto SelectionLookup::getAllCommonSelection(
    const std::vector<NodeSelection*>& first,
    const std::vector<NodeSelection*>& second) const noexcept
    -> std::vector<NodeSelection*>
{
    return utils::intersect(first, second);
}
