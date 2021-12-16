#pragma once
#incldue "impl.h"

class IFunctorImpl : public IImpl
{
    var result;
    bool isEvaluated = false;

public:
    virtual IFunctorImpl *toFunctorImpl() { return this; }
    inline static IFunctorImpl *cast(IImpl *impl) { return impl ? impl->toFunctorImpl() : nullptr; }
    std::wstring toString() const { return L""; }

public:
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
class _FunctorBind : public IFunctorImpl
{
    typedef typename std::decay<_Fx>::type _First;
    typedef std::tuple<typename std::decay<_Types>::type...> _Second;

    _First _func;
    _Second _args;

public:
    _FunctorBind(_Fx &&_Func, _Types &&..._Args)
        : _func(_Func), _args(std::forward<_Types>(_Args)...)
    { // construct from forwarded callable object and arguments
    }
    var eval()
    {
        return c17::apply(_func, _args);
    }
};