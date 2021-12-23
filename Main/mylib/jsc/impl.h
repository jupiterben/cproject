#pragma once
#include <stddef.h>
class String;
class IImpl
{
public:
	virtual ~IImpl() {}
};

class IValueImpl : public IImpl
{
public:
	virtual String toString() const = 0;
	virtual size_t getHash() const = 0;
};


//////////////////////////////////////////////////////////////////////////
template<class IT, class _HashFunc, class _EqualFunc>
class TValueImpl : public IValueImpl
{
public:
	typedef IT InternalType;
	typedef _HashFunc HashFunc;
	typedef _EqualFunc EqualFunc;

	TValueImpl(const InternalType& d) : internalData(d) {}
	const IT& internal()const { return internalData; }
	virtual size_t getHash() const { return GetDataHash(internalData); }
	virtual bool equalTo(const InternalType& d) const { return EqualFunc()(internalData, d); }

	inline static size_t GetDataHash(const InternalType &d) { return HashFunc()(d); }
protected:
	const IT internalData;
};
