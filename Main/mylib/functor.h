#pragma once
#include "impl.h"
#include "var.h"
#include "c17.hpp"
#include <functional>

class IFunctorImpl : public IImpl
{
    var result;
    bool isEvaluated = false;

public:
    virtual IFunctorImpl *toFunctor() { return this; }
	inline static IFunctorImpl* cast(IImpl* impl) { return impl ? impl->toFunctor() : nullptr; }
    inline var operator()(void)
    {
        if (!isEvaluated)
        {
            result = eval();
            isEvaluated = true;
        }
        return result;
    }
    virtual var eval() = 0;
};

// usage: var myFunction(var arg1, var arg2, ...){ ... }  Functor f(myFunction, a1, a2);
template <class _Fx, class... _Types>
class _Functor : public IFunctorImpl
{
    typedef typename std::decay<_Fx>::type _First;
    typedef std::tuple<typename std::decay<_Types>::type...> _Second;
    std::_Compressed_pair<_First, _Second> _Mypair;

public:
    _Functor(_Fx &&_Func, _Types &&..._Args) : _Mypair(std::_One_then_variadic_args_t(),
                                                       _STD forward<_Fx>(_Func), _STD forward<_Types>(_Args)...)
    { // construct from forwarded callable object and arguments
    }
    var eval()
    {
        return c17::apply(_Mypair._Get_first(), _Mypair._Get_second());
    }
};
template <class _Fx, class... _Types>
inline var Functor(_Fx &&_Func, _Types &&..._Args)
{
    typedef _Functor<_Fx, _Types...> FunctorType;
    return var::createInternal<FunctorType>(_Func, _Args...);
}
