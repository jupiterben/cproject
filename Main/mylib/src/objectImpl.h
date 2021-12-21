#pragma once
#include <jsc/impl.h>
#include <unordered_map>
#include <jsc/str.h>
#include "strImpl.h"

namespace std
{
	template <>
	struct hash<String>
	{
		std::size_t operator()(const String &p) const
		{
			using std::hash;
			using std::size_t;
			using std::string;
			return (std::size_t)p.getInternalPtr();
		}
	};
}
class ObjectImpl : public IImpl
{
public:
public:
	virtual String toString() const
	{
		StringStream ss;
		ss << "{";
		for (auto itr = internalData.begin(); itr != internalData.end(); ++itr)
		{
			if (itr != internalData.begin())
				ss << ",";
			const String &key = itr->first;
			const var &value = itr->second;
			ss << key.toString() << ": " << value.toString();
		}
		ss << "}";
		return ss.str();
	}

public:
	ObjectImpl(const InitialMapType &keyValues) : internalData(keyValues.begin(), keyValues.end()) {}
	ObjectImpl(InitialListType initial_list) : internalData(initial_list) {}

protected:
	typedef std::unordered_map<String, var> InternalType;
	const InternalType internalData;
};