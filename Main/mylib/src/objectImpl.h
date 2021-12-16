#pragma once
#include <jsc/impl.h>
#include <unordered_map>
#include <jsc/str.h>
#include <sstream>

namespace std
{
	template <>
	struct hash<String> {
		std::size_t operator()(const String& p) const {
			using std::size_t;
			using std::hash;
			using std::string;
			return (std::size_t)p.getInternalPtr();
		}
	};
}
class ObjectImpl : public IImpl
{
public:
	
public:
    virtual ObjectImpl *toObjectImpl() { return this; }
    inline static ObjectImpl *cast(IImpl *impl) { return impl ? impl->toObjectImpl() : nullptr; }
	virtual std::wstring toString() const 
	{ 
		 std::wstringstream ss;
		 ss << L"{";
		 for(auto itr = internalData.begin(); itr!= internalData.end(); ++itr)
		 {
			if(itr != internalData.begin()) ss << ",";
			const String& key = itr->first;
			const var& value = itr->second;
			ss << key << L": " << value;
		 }
		 ss << L"}";
		 return ss.str();
	}
public:
	ObjectImpl(const InitialMapType& keyValues) : internalData(keyValues.begin(), keyValues.end()){}
	ObjectImpl(InitialListType initial_list):internalData(initial_list){}

protected:
	typedef std::unordered_map<String, var> InternalType;
	const InternalType internalData;
};