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

class ObjectImpl : public IValue
{
public:
	typedef std::unordered_map<String, var> InternalType;
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
	virtual size_t getHash()const
    {
        return 0;
    }
public:
	ObjectImpl(const InitialMapType &keyValues) : internalData(keyValues.begin(), keyValues.end()) {}
	ObjectImpl(InitialListType initial_list) : internalData(initial_list) {}

protected:
	const InternalType internalData;
};