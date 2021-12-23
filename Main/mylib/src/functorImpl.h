#pragma once
#include <jsc/impl.h>
#include "c17.hpp"
#include <jsc/str.h>

class IFunctorImpl : public IImpl
{
    var result;
    bool isEvaluated = false;
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
template<class _Fx, class... _Types> 
struct _FnInternalType
{
    typedef typename std::decay<_Fx>::type _First;
    typedef std::tuple<typename std::decay<_Types>::type...> _Second;
    typedef std::pair<_First, _Second> Type;
    Type data;
};

template <class _Fx, class... _Types>
class _FunctorBind : public IFunctorImpl
, public TValueImpl<typename _FnInternalType<_Fx, typename _Types...>::Type>
{
public:
    using TValueImpl::TValueImpl;
    var eval()
    {
        return c17::apply(internalData.first, internalData.second);
    }
};