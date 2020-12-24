#include <execution>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <graph/GridGraph.hpp>
#include <graph/Node.hpp>
#include <map>
#include <pathfinding/Distance.hpp>
#include <queue>
#include <random>
#include <selection/NodeSelection.hpp>
#include <selection/NodeSelectionCalculator.hpp>
#include <selection/SelectionLookup.hpp>
#include <unordered_set>
#include <utils/Range.hpp>
#include <utils/Utils.hpp>
#include <vector>

using selection::SelectionLookup;
using selection::NodeSelection;

SelectionLookup::SelectionLookup(const graph::GridGraph& graph,
                                 std::vector<NodeSelection> selections)
    : graph_(graph),
      selections_(std::move(selections)),
      left_selections_(graph_.size()),
      right_selections_(graph_.size())
{
    for(const auto& [i, selection] : utils::enumerate(selections_)) {
        for(auto node : selection.getLeftSelection()) {
            auto idx = graph_.nodeToIndex(node);
            left_selections_[idx].emplace_back(i);
        }

        for(auto node : selection.getRightSelection()) {
            auto idx = graph_.nodeToIndex(node);
            right_selections_[idx].emplace_back(i);
        }
    }
}

SelectionLookup::SelectionLookup(const graph::GridGraph& graph,
                                 std::vector<NodeSelection> selections,
                                 std::vector<std::vector<std::size_t>> left_selections,
                                 std::vector<std::vector<std::size_t>> right_selections)
    : graph_(graph),
      selections_(std::move(selections)),
      left_selections_(std::move(left_selections)),
      right_selections_(std::move(right_selections)) {}


auto SelectionLookup::getAllCommonSelection(const graph::Node& first,
                                            const graph::Node& second) const noexcept
    -> utils::CRefVec<NodeSelection>
{
    auto first_idx = graph_.nodeToIndex(first);
    auto second_idx = graph_.nodeToIndex(second);

    const auto& first_left_selections = left_selections_[first_idx];
    const auto& second_right_selections = right_selections_[second_idx];
    const auto& first_right_selections = right_selections_[first_idx];
    const auto& second_left_selections = left_selections_[second_idx];

    auto left_to_right = getAllCommonSelection(first_left_selections, second_right_selections);
    auto right_to_left = getAllCommonSelection(first_right_selections, second_left_selections);

    auto all = utils::concat(std::move(left_to_right),
                             std::move(right_to_left));

    utils::CRefVec<NodeSelection> all_refs;
    all_refs.reserve(all.size());

    std::transform(std::begin(all),
                   std::end(all),
                   std::back_inserter(all_refs),
                   [&](auto idx) {
                       return std::cref(selections_[idx]);
                   });

    return all_refs;
}

auto SelectionLookup::getOneCommonSelection(
    const std::vector<std::size_t>& first,
    const std::vector<std::size_t>& second) const noexcept
    -> std::optional<std::size_t>
{
    auto iter1 = std::cbegin(first);
    auto iter2 = std::cbegin(second);
    auto iter1_end = std::cend(first);
    auto iter2_end = std::cend(second);

    while(iter1 != iter1_end and iter2 != iter2_end) {
        if(*iter1 < *iter2) {
            ++iter1;
        } else if(*iter2 < *iter1) {
            ++iter2;
        } else {
            return *iter1;
        }
    }

    return std::nullopt;
}

auto SelectionLookup::getAllCommonSelection(
    const std::vector<std::size_t>& first,
    const std::vector<std::size_t>& second) const noexcept
    -> std::vector<std::size_t>
{
    return utils::intersect(first, second);
}


auto SelectionLookup::getSizeDistributionLeft() const noexcept
    -> std::map<std::size_t, std::size_t>
{
    std::map<std::size_t, std::size_t> ret_map;
    for(auto n : graph_) {
        auto idx = graph_.nodeToIndex(n);
        const auto& s = left_selections_[idx];

        auto iter = ret_map.find(s.size());

        if(iter == std::end(ret_map)) {
            ret_map[s.size()] = 1;
        } else {
            iter->second++;
        }
    }

    return ret_map;
}

auto SelectionLookup::getSizeDistributionRight() const noexcept
    -> std::map<std::size_t, std::size_t>
{
    std::map<std::size_t, std::size_t> ret_map;
    for(auto n : graph_) {
        auto idx = graph_.nodeToIndex(n);
        const auto& s = right_selections_[idx];

        auto iter = ret_map.find(s.size());


        if(iter == std::end(ret_map)) {
            ret_map[s.size()] = 1;
        } else {
            iter->second++;
        }
    }

    return ret_map;
}

auto SelectionLookup::getSizeDistributionTotal() const noexcept
    -> std::map<std::size_t, std::size_t>
{

    std::map<std::size_t, std::size_t> ret_map;
    for(auto n : graph_) {
        auto idx = graph_.nodeToIndex(n);
        const auto& s = left_selections_[idx];

        auto iter = ret_map.find(s.size());

        if(iter == std::end(ret_map)) {
            ret_map[s.size()] = 1;
        } else {
            iter->second++;
        }
    }

    for(auto n : graph_) {
        auto idx = graph_.nodeToIndex(n);
        const auto& s = right_selections_[idx];

        auto iter = ret_map.find(s.size());

        if(iter == std::end(ret_map)) {
            ret_map[s.size()] = 1;
        } else {
            iter->second++;
        }
    }
    return ret_map;
}


auto SelectionLookup::sanityCheck() const noexcept
    -> bool
{
    for(auto n : graph_) {
        auto n_idx = graph_.nodeToIndex(n);
        const auto& left_selections = left_selections_[n_idx];
        const auto& right_selections = right_selections_[n_idx];

        std::unordered_set<graph::Node> target_nodes;

        for(auto left_idx : left_selections) {
            const auto& targets = selections_[left_idx].getRightSelection();
            target_nodes.insert(std::begin(targets),
                                std::end(targets));
        }

        for(auto right_idx : right_selections) {
            const auto& targets = selections_[right_idx].getLeftSelection();
            target_nodes.insert(std::begin(targets),
                                std::end(targets));
        }

        auto should_have = graph_.countWalkableNodes() - graph_.getWalkableNeigbours(n).size() - 1;
        auto has = target_nodes.size();

        if(should_have > has) {
            fmt::print("should have: {}, has: {}\n", should_have, has);
            return false;
        }
    }

    return true;
}
