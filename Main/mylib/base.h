#pragma once
#include <memory>
#include <string>
#include <functional>

class NumberImpl;
class IFunctorImpl;
class IImpl
{
public:
    virtual IFunctorImpl *toFunctor() { return nullptr; }
    virtual NumberImpl* toNumber() { return nullptr; }
};


class NumberImpl : public IImpl
{
	friend class var;
private:
	double x;
public:
	inline NumberImpl(double _x) : x(_x) {}
	operator double() const { return x; }
	virtual NumberImpl* toNumber() { return this; }
protected:
	var operator+(const NumberImpl& other)const;
	var operator-(const NumberImpl& other)const;
	var operator*(const NumberImpl& other)const;
	var operator/(const NumberImpl& other)const;
};

class StringImpl : public IImpl
{
private:
	std::wstring x;
};

class ArrayImpl : public IImpl
{
private:
public:
};

template<class T>
class VarT
{
protected:
    typedef std::shared_ptr<T> InternalType;
    InternalType internalPtr;
    VarT(){}
    VarT(InternalType internal) : internalPtr(internal) {}
public:
    inline bool isUndefined() const{ return internalPtr.get() == nullptr; }
};

class var : public VarT<IImpl>
{
public:
	const static var undefined;
public:
	var() {}
	var(double x) :VarT(createInternal<NumberImpl>(x)) {}
	var(const InternalType internal) :VarT(internal) {}
	bool operator == (var other)const
	{
		return internalPtr == other.internalPtr;
	}
	var operator()(void);
	var operator+(var other)const;
	var operator-(var other)const;
	var operator*(var other)const;
	var operator/(var other)const;

	template<class _Ty, class... _Types>
	inline static var createInternal(_Types&&... _Args)
	{	// make a shared_ptr
		return var(std::make_shared<_Ty>(_Args...));
	}
protected:
	inline NumberImpl * toNumber() const
	{
		return isUndefined() ? nullptr : internalPtr->toNumber();
	}
	inline IFunctorImpl * toFunctor() const
	{
		return isUndefined() ? nullptr : internalPtr->toFunctor();
	}
};

class Number : VarT<NumberImpl>
{
public:
	Number() {}
};


class IFunctorImpl : public IImpl
{
	var result;
	bool isEvaluated = false;
public:
	virtual IFunctorImpl *toFunctor() { return this; }
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

template<class _Ret, class _Fx, class ..._Types>
class _Functor
{
	typedef tuple<typename decay<_Types>::type...> ArgsType;
	typedef typename decay<_Fx>::type FxType;
public:
	_Functor(_Fx&& _Func, _Types&&... _Args)
	{

	}
};


template<class _Ret,
	class _Fx,
	class... _Types>
	class _Binder
	: public _Binder_result_type<_Ret, _Fx>::type
{	// wrap bound callable object and arguments
private:
	typedef make_integer_sequence<size_t, sizeof...(_Types)> _Seq;
	typedef typename decay<_Fx>::type _First;
	typedef tuple<typename decay<_Types>::type...> _Second;

	_Compressed_pair<_First, _Second> _Mypair;

public:
	explicit _Binder(_Fx&& _Func, _Types&&... _Args)
		: _Mypair(_One_then_variadic_args_t(),
			_STD forward<_Fx>(_Func), _STD forward<_Types>(_Args)...)
	{	// construct from forwarded callable object and arguments
	}

#define _BINDER_OPERATOR(CONST_OPT) \
	template<class... _Unbound> \
		auto operator()(_Unbound&&... _Unbargs) CONST_OPT \
		-> decltype(_Call_binder(_Forced<_Ret>(), _Seq(), \
			_Mypair._Get_first(), _Mypair._Get_second(), \
			_STD forward_as_tuple(_STD forward<_Unbound>(_Unbargs)...))) \
		{	/* invoke bound callable object with bound/unbound arguments */ \
		return (_Call_binder(_Forced<_Ret>(), _Seq(), \
			_Mypair._Get_first(), _Mypair._Get_second(), \
			_STD forward_as_tuple(_STD forward<_Unbound>(_Unbargs)...))); \
		}

	_CLASS_DEFINE_CONST(_BINDER_OPERATOR)
#undef _BINDER_OPERATOR
};

#define functor(fname, fbody)            \
    class __##fname : public IFunctorImpl \
    {                                     \
    public:                               \
        __##fname() {}                    \
        var eval() fbody                  \
    };                                    \
    inline var fname() { return var::createInternal<__##fname>(); }

#define functor1(fname, p1, fbody)             \
    class __##fname : public IFunctorImpl     \
    {                                         \
        var p1;                               \
                                              \
    public:                                   \
        __##fname(var __##p1) : p1(__##p1) {} \
        var eval() fbody                      \
    };                                        \
    inline var fname(var p1) { return var::createInternal<__##fname>(p1); }

#define functor2(fname, p1, p2, fbody)                                 \
    class __##fname : public IFunctorImpl                             \
    {                                                                 \
        var p1;                                                       \
        var p2;                                                       \
                                                                      \
    public:                                                           \
        __##fname(var __##p1, var __##p2) : p1(__##p1), p2(__##p2) {} \
        var eval() fbody                                              \
    };                                                                \
    inline var fname(var p1, var p2) { return var::createInternal<__##fname>(p1, p2); }
