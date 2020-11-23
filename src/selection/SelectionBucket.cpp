
#include <graph/Node.hpp>
#include <numeric>
#include <selection/NodeSelection.hpp>
#include <selection/SelectionBucket.hpp>
#include <vector>

using selection::SelectionBucket;
using selection::NodeSelection;

SelectionBucket::SelectionBucket(std::vector<NodeSelection> selections)
    : selections_(std::move(selections))
{
    std::sort(std::begin(selections_),
              std::end(selections_));
}

auto SelectionBucket::weight() const noexcept
    -> std::size_t
{
    return std::accumulate(std::begin(selections_),
                           std::end(selections_),
                           0,
                           [](auto init, const auto& sel) {
                               return init + sel.weight();
                           });
}

auto SelectionBucket::isSubSetOf(const SelectionBucket& other) const noexcept
    -> bool
{
    return isSubSetOf(other.selections_);
}

auto SelectionBucket::isSubSetOf(const std::vector<NodeSelection>& other) const noexcept
    -> bool
{
    return std::all_of(std::begin(selections_),
                       std::end(selections_),
                       [&](const auto& selection) {
                           return std::find_if(std::begin(other),
                                               std::end(other),
                                               [&](const auto& elem) {
                                                   return elem.getIndex() == selection.getIndex();
                                               })
                               != std::end(other);
                       });
}

auto SelectionBucket::isSuperSetOf(const SelectionBucket& other) const noexcept
    -> bool
{
    return isSuperSetOf(other.selections_);
}

auto SelectionBucket::isSuperSetOf(const std::vector<NodeSelection>& other) const noexcept
    -> bool
{
    return std::all_of(std::begin(other),
                       std::end(other),
                       [&](const auto& selection) {
                           return contains(selection);
                       });
}

auto SelectionBucket::contains(const NodeSelection& other) const noexcept
    -> bool
{
    return std::find_if(std::begin(selections_),
                        std::end(selections_),
                        [&](const auto& elem) {
                            return elem.getIndex() == other.getIndex();
                        })
        != std::end(selections_);
}

auto SelectionBucket::canAnswer(const graph::Node& from,
                                const graph::Node& to) const noexcept
    -> bool
{
    return std::any_of(std::begin(selections_),
                       std::end(selections_),
                       [&](const auto& selection) {
                           return selection.canAnswer(from, to);
                       });
}

auto SelectionBucket::merge(const SelectionBucket& other) const noexcept
    -> SelectionBucket
{
    auto merged = utils::intersect(selections_,
                                   other.selections_);
    return SelectionBucket{std::move(merged)};
}

auto SelectionBucket::getSelections() const noexcept
    -> const std::vector<NodeSelection>&
{
    return selections_;
}