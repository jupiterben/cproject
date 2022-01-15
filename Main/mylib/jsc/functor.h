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

#include "./functorImpl.h"
class _SwitchCaseImpl;
class SwitchBuilder
{
private:
	std::shared_ptr<_SwitchCaseImpl> m_pImpl;

public:
	SwitchBuilder(const var &v)
		: m_pImpl(std::make_shared<_SwitchCaseImpl>())
	{
		m_pImpl->evalValue = v;
	}
	SwitchBuilder &Case(const var &cond, const var &v)
	{
		m_pImpl->addCase(cond, v);
		return *this;
	}
	var Default(const var &v)
	{
		m_pImpl->defaultValue = v;
		return var(m_pImpl);
	}
};
typedef SwitchBuilder Switch;