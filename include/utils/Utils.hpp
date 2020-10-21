#pragma once

#include <algorithm>
#include <fmt/core.h>
#include <future>
#include <utility>
#include <vector>

namespace utils {

template<class Head0, class Head1, class... Tail>
constexpr auto min(Head0&& head0, Head1&& head1, Tail&&... tail) noexcept
{
    if constexpr(sizeof...(tail) == 0) {
        return head0 < head1 ? head0 : head1;
    } else {
        return min(
            min(std::forward<Head0>(head0),
                std::forward<Head1>(head1)),
            std::forward<Tail>(tail)...);
    }
}

template<class Head0, class Head1, class... Tail>
constexpr auto max(Head0&& head0, Head1&& head1, Tail&&... tail) noexcept
{
    if constexpr(sizeof...(tail) == 0) {
        return head0 > head1 ? head0 : head1;
    } else {
        return max(
            max(std::forward<Head0>(head0),
                std::forward<Head1>(head1)),
            std::forward<Tail>(tail)...);
    }
}

template<class Head0, class Head1, class... Tail>
constexpr auto concat(Head0&& head0, Head1&& head1, Tail&&... tail) noexcept
{
    if constexpr(sizeof...(tail) == 0) {
        head0.insert(std::end(head0),
                     std::begin(head1),
                     std::end(head1));
        return std::forward<Head0>(head0);
    } else {
        return concat(
            concat(std::forward<Head0>(head0),
                   std::forward<Head1>(head1)),
            std::forward<Tail>(tail)...);
    }
}

template<class T>
auto collectFutures(std::vector<std::future<T>>&& futures) noexcept
    -> std::vector<T>
{
    std::vector<T> collected;
    collected.reserve(futures.size());

    std::transform(std::make_move_iterator(std::begin(futures)),
                   std::make_move_iterator(std::end(futures)),
                   std::back_inserter(collected),
                   [](auto future) {
                       return future.get();
                   });

    return collected;
}


template<class Head0, class Head1, class... Tail>
auto await(std::future<Head0>&& head0,
           std::future<Head1>&& head1,
           std::future<Tail>&&... tail) noexcept
    -> std::tuple<Head0, Head1, Tail...>
{
    return std::tuple{head0.get(),
                      head1.get(),
                      tail.get()...};
}

template<class T, class... Tail>
auto intersect(std::vector<T>&& head0,
               std::vector<T>&& head1,
               Tail&&... tail) noexcept
    -> std::vector<T>
{
    if constexpr(sizeof...(tail) == 0) {
        std::vector<T> intersection;
        std::set_intersection(std::make_move_iterator(std::begin(head0)),
                              std::make_move_iterator(std::end(head0)),
                              std::make_move_iterator(std::begin(head1)),
                              std::make_move_iterator(std::end(head1)),
                              std::back_inserter(intersection));
        return intersection;
    } else {
        return intersect(
            intersect(std::move(head0),
                      std::move(head1)),
            std::forward<Tail>(tail)...);
    }
}

template<class T, class... Tail>
auto intersect(const std::vector<T>& head0,
               const std::vector<T>& head1,
               Tail&&... tail) noexcept
    -> std::vector<T>
{
    if constexpr(sizeof...(tail) == 0) {
        std::vector<T> intersection;
        std::set_intersection(std::begin(head0),
                              std::end(head0),
                              std::begin(head1),
                              std::end(head1),
                              std::back_inserter(intersection));
        return intersection;
    } else {
        return intersect(
            intersect(head0,
                      head1),
            std::forward<Tail>(tail)...);
    }
}




} // namespace utils
