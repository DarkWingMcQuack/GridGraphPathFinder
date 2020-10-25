#pragma once

#include <graph/GridCell.hpp>
#include <mutex>
#include <unordered_set>

namespace separation {

class WellSeparationCalculatorCache
{
public:
    auto checkAndMarkCalculation(graph::GridCell first,
                                 graph::GridCell second)
        -> bool
    {
        std::lock_guard lock{mtx_};
        auto inserted =
            cache_.find(std::pair{first, second}) != std::end(cache_)
            or cache_.find(std::pair{second, first}) != std::end(cache_);

        if(!inserted) {
            cache_.emplace(first, second);
        }

        return inserted;
    }

private:
    std::mutex mtx_;
    std::unordered_set<std::pair<graph::GridCell, graph::GridCell>> cache_;
};

} // namespace separation
