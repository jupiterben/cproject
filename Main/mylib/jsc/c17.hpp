#pragma once
#include <type_traits>

namespace c17
{
    template <typename _Ret, typename F, typename Tuple, std::size_t... I>
    inline _Ret apply_impl(F &&f, Tuple &&t, std::index_sequence<I...>)
    {
        return std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...);
    }
    template <typename _Ret, typename F, typename Tuple>
    inline _Ret apply(F &&f, Tuple &&t)
    {
        using Indices = std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>;
        return apply_impl<_Ret>(std::forward<F>(f), std::forward<Tuple>(t), Indices());
    }
}
