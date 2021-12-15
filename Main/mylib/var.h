#pragma once
#include <memory>
#include "impl.h"

template <typename T>
class VarT
{
public:
	typedef VarT<T> type;
    typedef std::shared_ptr<T> InternalPtrType;
	InternalPtrType internalPtr;

public:
    VarT() {}
    VarT(InternalPtrType internal) : internalPtr(internal) {}
	template<class T2>
	VarT(const VarT<T2>& other) : internalPtr(other.internalPtr, other.toImpl<T>()) {}

	template <class _TyImpl, class... _Types>
	inline static type createInternal(_Types &&..._Args)
	{ // make a shared_ptr
		return VarT(std::make_shared<_TyImpl>(_Args...));
	}
	inline T* getInternalPtr() const{ return internalPtr.get(); }
	
	template<class T2>
	inline T2* toImpl()const { return T2::cast(getInternalPtr()); }
public:
    inline bool isUndefined() const { return internalPtr.get() == nullptr; }
};

class var : public VarT<IImpl>
{
public:
    const static var undefined;

public:
    var() {}
	var(double x) :VarT(createInternal<NumberImpl>(x)) {}
	var(const type& t) :VarT(t) {}
	var(std::initializer_list<var> list);

	template<class T>
	var(const VarT<T>& other) : VarT(other) {}

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
