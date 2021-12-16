#pragma once
#include "impl.h"
#include "var.h"

class Functor : public var
{
public:
	Functor(const var &v);
	// 	template <class _Fx, class... _Types>
	// 	Functor(_Fx &&_Func, _Types &&..._Args)
	// 	{
	// 		typedef _FunctorBind<_Fx, _Types...> FunctorType;
	// 		var(createInternal<FunctorType>(_Func, _Args...));
	// 	}
};
