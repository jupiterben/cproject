#pragma once
#include "impl.h"
#include "var.h"
#include "functorImpl.h"

class Functor : public var
{
public:
	Functor(const var &v);

	template <class _Fx, class... _Types>
	Functor(_Fx&& fx, _Types... args)
	{
		typedef _FunctorBind<_Fx, _Types...> _FnBind;
		typedef TValueImplPool<typename _FnBind> PoolType;
		typedef _FnBind::InternalType InternalType;
		auto& pool = PoolType::GetInstance();
		InternalType internalData(std::forward<_Fx>(fx), std::make_tuple(args...));
		var(pool.GetOrCreate(internalData));
	}
};
