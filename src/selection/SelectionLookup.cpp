#include <fmt/core.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <pathfinding/Distance.hpp>
#include <queue>
#include <random>
#include <selection/SelectionLookup.hpp>
#include <selection/NodeSelection.hpp>
#include <selection/NodeSelectionCalculator.hpp>
#include <unordered_set>
#include <utils/Utils.hpp>
#include <vector>

using selection::SelectionLookup;
using selection::NodeSelection;

SelectionLookup::SelectionLookup(std::vector<NodeSelection> selections)
    : selections_(std::move(selections))
{
    for(auto& selection : selections_) {
        for(auto node : selection.getLeftSelection()) {
            node_selections_map_[node].emplace_back(&selection);
        }

        for(auto node : selection.getRightSelection()) {
            node_selections_map_[node].emplace_back(&selection);
        }
    }

    for(auto& [_, selections] : node_selections_map_) {
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
    return std::pair{
        node_selections_map_.at(first),
        node_selections_map_.at(second)};
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

auto SelectionLookup::getAllCommonSelection(
    const std::vector<NodeSelection*>& first,
    const std::vector<NodeSelection*>& second) const noexcept
    -> std::vector<NodeSelection*>
{
    return utils::intersect(first, second);
}








