#pragma once
#include <type_traits>
#include <sstream>
#include <string>
#include <locale>
#include <codecvt>

namespace c17
{
    template <typename F, typename Tuple, std::size_t... I>
    inline auto apply_impl(F &&f, Tuple &&t, std::index_sequence<I...>)
    {
        return std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...);
    }
    template <typename F, typename Tuple>
    inline auto apply(F &&f, Tuple &&t)
    {
        using Indices = std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>;
        return apply_impl(std::forward<F>(f), std::forward<Tuple>(t), Indices());
    }

    template <class InputVarIt>
    inline TStr join(InputVarIt first, InputVarIt last,
                      const TStr &separator, const TStr &concluder=_TS(""))
    {
        if (first == last)
        {
            return concluder;
        }

		TStrStream ss;
        ss << (*first);
        ++first;
        while (first != last)
        {
            ss << separator;
            ss << (*first);
            ++first;
        }
        ss << concluder;
        return TStr(ss.str().c_str());
    }
}
