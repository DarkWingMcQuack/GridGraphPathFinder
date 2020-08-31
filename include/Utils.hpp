#pragma once

namespace util {

template<class Head0, class Head1, class... Tail>
constexpr auto min(Head0&& head0, Head1&& head1, Tail&&... tail)
{
    if constexpr(sizeof...(tail) == 0) {
        return head0 < head1 ? head0 : head1;
    } else {
        return min(min(head0, head1), tail...);
    }
}

template<class Head0, class Head1, class... Tail>
constexpr auto max(Head0&& head0, Head1&& head1, Tail&&... tail)
{
    if constexpr(sizeof...(tail) == 0) {
        return head0 > head1 ? head0 : head1;
    } else {
        return min(min(head0, head1), tail...);
    }
}

} // namespace util
