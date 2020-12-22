
#include <algorithm>
#include <graph/Node.hpp>
#include <numeric>
#include <selection/NodeSelection.hpp>
#include <selection/SelectionBucket.hpp>
#include <utils/Utils.hpp>
#include <vector>

using selection::SelectionBucket;

SelectionBucket::SelectionBucket(std::vector<std::size_t> selections,
                                 bool sort)
    : selections_(std::move(selections))
{
    if(sort) {
        std::sort(std::begin(selections_),
                  std::end(selections_));
    }
}

auto SelectionBucket::isSubSetOf(const SelectionBucket& other) const noexcept
    -> bool
{
    return isSubSetOf(other.selections_);
}

auto SelectionBucket::isSubSetOf(const std::vector<std::size_t>& other) const noexcept
    -> bool
{
    return std::all_of(std::begin(selections_),
                       std::end(selections_),
                       [&](const auto& selection) {
                           return std::binary_search(std::begin(other),
                                                     std::end(other),
                                                     selection);
                       });
}

auto SelectionBucket::isSuperSetOf(const SelectionBucket& other) const noexcept
    -> bool
{
    return isSuperSetOf(other.selections_);
}

auto SelectionBucket::isSuperSetOf(const std::vector<std::size_t>& other) const noexcept
    -> bool
{
    return std::all_of(std::begin(other),
                       std::end(other),
                       [&](const auto& selection) {
                           return contains(selection);
                       });
}

auto SelectionBucket::contains(const std::size_t& other) const noexcept
    -> bool
{
    return std::binary_search(std::begin(selections_),
                              std::end(selections_),
                              other);
}

auto SelectionBucket::getCommonSelection(const SelectionBucket& other) const noexcept
    -> std::optional<std::size_t>
{
    auto iter1 = std::begin(selections_);
    auto iter2 = std::begin(other.selections_);
    auto iter1_end = std::end(selections_);
    auto iter2_end = std::end(other.selections_);

    while(iter1 != iter1_end && iter2 != iter2_end) {
        if(*iter1 < *iter2) {
            ++iter1;
        } else if(*iter2 < *iter1) {
            ++iter2;
        } else {
            return *iter2;
        }
    }
    return std::nullopt;
}

auto SelectionBucket::merge(SelectionBucket other) && noexcept
    -> SelectionBucket
{
    auto merged = utils::intersect(std::move(selections_),
                                   std::move(other.selections_));
    return SelectionBucket{std::move(merged), false};
}

auto SelectionBucket::getSelections() const noexcept
    -> const std::vector<std::size_t>&
{
    return selections_;
}

auto SelectionBucket::exclude(std::size_t selection) && noexcept
    -> SelectionBucket
{
    selections_.erase(std::remove(std::begin(selections_),
                                  std::end(selections_),
                                  selection),
                      std::end(selections_));

    return SelectionBucket{std::move(selections_),
                           false};
}

auto SelectionBucket::getFirstIndex() const noexcept
    -> std::optional<std::size_t>
{
    if(selections_.empty()) {
        return std::nullopt;
    }

    return selections_.front();
}

auto SelectionBucket::getLastIndex() const noexcept
    -> std::optional<std::size_t>
{
    if(selections_.empty()) {
        return std::nullopt;
    }

    return selections_.back();
}

auto SelectionBucket::size() const noexcept
    -> std::size_t
{
    return selections_.size();
}
