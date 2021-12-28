#pragma once
#include <jsc/impl.h>
#include <jsc/str.h>

class IFunctorImpl : virtual public IImpl
{
    var result;
    bool isEvaluated = false;
    mutable std::mutex _mtx;

public:
    inline var operator()(void)
    {
        std::lock_guard<std::mutex> l(_mtx);
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
#include <functional>
template <class _Fx, class... _Types>
struct _FnTypeTrait
{
    using _First = std::decay_t<_Fx>;
    using _Second = std::tuple<std::decay_t<_Types>...>;
    typedef std::pair<_First, _Second> Type;
};

#include "c17.hpp"
template <class _Fx, class... _Types>
class _FunctorBind : public IFunctorImpl, public TValueImpl<typename _FnTypeTrait<_Fx, _Types...>::Type>
{
public:
    typedef _FnTypeTrait<_Fx, _Types...> Trait;
    typedef typename Trait::Type InternalType;
    _FunctorBind(const InternalType &d, size_t hash) : TValueImpl<InternalType>(d, hash) {}
    var eval()
    {
        return c17::apply<var>(this->internalData.first, this->internalData.second);
    }
    String toString() const { return _U("[Functor]"); }

    static size_t hash(const InternalType &d)
    {
        return 0;
    }
};