#pragma once
#include "impl.h"
#include "var.h"
#include "functorImpl.h"
#include <functional>

class Functor : public var
{
public:
	Functor(const var &v);

	template <class _Fx, class... _Types>
	Functor(_Fx &&fx, _Types... args) : var(Create(fx, args...)) {}

protected:
	template <class _Fx, class... _Types>
	InternalPtrType Create(_Fx &&fx, _Types... args)
	{
		typedef _FunctorBind<_Fx, _Types...> _FnBind;
		typedef TValueImplPool<_FnBind> _PoolType;
		typedef typename _FnBind::InternalType InternalType;
		auto &pool = _PoolType::Instance();
		InternalType internalData(std::forward<_Fx>(fx), std::make_tuple(args...));
		return pool.GetOrCreate(internalData);
	}
};
