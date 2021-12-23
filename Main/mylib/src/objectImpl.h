#pragma once
#include <jsc/impl.h>
#include <unordered_map>
#include <jsc/str.h>
#include "strImpl.h"

template <>
struct std::hash<String>
{
	std::size_t operator()(const String &str) const noexcept
	{
		return str.getHash();
	}
};
typedef std::unordered_map<String, var> ObjectInternalType;

struct ObjectInternalHash
{
	std::size_t operator()(const ObjectInternalType &data) const noexcept
	{
		for (auto itr = data.begin(); itr != data.end(); ++itr)
		{

		}
		return 0;
	}
};

class ObjectImpl : public TValueImpl<ObjectInternalType, ObjectInternalHash, std::equal_to<ObjectInternalType>>
{
public:
	using TValueImpl::TValueImpl;
	virtual String toString() const
	{
		StringStream ss;
		ss << _U("{");
		for (auto itr = internalData.begin(); itr != internalData.end(); ++itr)
		{
			if (itr != internalData.begin())
				ss << _U(",");
			const String &key = itr->first;
			const var &value = itr->second;
			ss << key.toString() << _U(": ") << value.toString();
		}
		ss << _U("}");
		return ss.str();
	}
public:
};