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
template <class IType>
class TValueImpl : public IValueImpl
{
public:
	typedef IType InternalType;

	TValueImpl(const InternalType &d, size_t hash)
		: internalData(d), _hash(hash) {}
	inline const InternalType &internal() const { return internalData; }
	virtual size_t getHash() const { return _hash; }
	virtual bool equalTo(const InternalType &d) const
	{
		return std::equal_to<InternalType>{}(internalData, d);
	}
protected:
	const InternalType internalData;
	size_t _hash;
};
