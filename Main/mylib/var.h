#pragma once
#include "impl.h"
#include <initializer_list>
#include <memory>

template <class T>
class VarT
{
public:
	typedef VarT<T> type;
	typedef std::shared_ptr<T> InternalPtrType;
	InternalPtrType internalPtr;

	inline T *getInternalPtr() const { return internalPtr.get(); }
	template <class T2>
	inline T2 *toImpl() const
	{
		return T2::cast(getInternalPtr());
	}

public:
	VarT() {}
	VarT(InternalPtrType internal) : internalPtr(internal) {}

	template <class T2>
	VarT(const VarT<T2> &other)
		: internalPtr(other.internalPtr, other.template toImpl<T>()) {}

	template <class T2, class... _Types>
	inline static auto createInternal(_Types &&..._Args)
	{ // make a shared_ptr
		return VarT<T2>(std::make_shared<T2>(_Args...));
	}

public:
	inline bool isUndefined() const { return internalPtr.get() == nullptr; }
};

class var : public VarT<IImpl>
{
public:
	const static var undefined;

public:
	var() {}
	var(double x);
	var(const type &t) : VarT(t) {}
	var(std::initializer_list<var> list);

	inline bool operator==(var other) const
	{
		return internalPtr == other.internalPtr;
	}
	var operator()(void);
	var operator+(var other) const;
	var operator-(var other) const;
	var operator*(var other) const;
	var operator/(var other) const;
};
