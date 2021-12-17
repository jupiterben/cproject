#pragma once
#include "impl.h"
#include <initializer_list>
#include <memory>
#include <string>

class String;
class var 
{
public:
	const static var undefined;

	typedef std::shared_ptr<IImpl> InternalPtrType;
	InternalPtrType internalPtr;
	inline IImpl *getInternalPtr() const { return internalPtr.get(); }

	template<class T2>
	inline T2* getImpl() const { return T2::cast(getInternalPtr()); }

	inline bool isUndefined() const { return getInternalPtr() == nullptr; }

protected:
	var(const InternalPtrType& ptr) :internalPtr(ptr) {}

	template <class _ImplType, class... _Types>
	inline static var createInternal(_Types &&..._Args)
	{ // make a shared_ptr
		return var(std::make_shared<_ImplType>(_Args...));
	}
public:
	var() {}
	var(double x);
	var(const TStr& s);
	var(const var& other) :internalPtr(other.internalPtr) {}
	template<class T2>
	var(const var& other, T2* ptr) : internalPtr(other.internalPtr, ptr) {}
	inline bool operator==(var other) const
	{
		return internalPtr == other.internalPtr;
	}
	var operator()(void);

	String toString()const;

	TOstream& toStream(TOstream& os)const;
};

inline TOstream& operator<<(TOstream& os, const var& dt)
{
    return dt.toStream(os);
}
