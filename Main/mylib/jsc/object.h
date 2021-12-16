#pragma once
#include "impl.h"
#include <map>
#include "var.h"
#include "str.h"
#include <unordered_map>

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
	typedef std::initializer_list<std::pair<const String, var>> InitialListType;
	typedef std::map<String, var> InitialMapType;
public:
    virtual ObjectImpl *toObjectImpl() { return this; }
    inline static ObjectImpl *cast(IImpl *impl) { return impl ? impl->toObjectImpl() : nullptr; }
	virtual std::wstring toString() const { return L""; }
public:
	ObjectImpl(const InitialMapType& keyValues) : internalData(keyValues.begin(), keyValues.end()){}
	ObjectImpl(InitialListType initial_list):internalData(initial_list){}

protected:
	typedef std::unordered_map<String, var> InternalType;
	const InternalType internalData;
};

class Object : public var
{
public:
	Object(const ObjectImpl::InitialMapType& keyValues) : var(createInternal<ObjectImpl>(keyValues)) {}
	Object(ObjectImpl::InitialListType initList) :var(createInternal<ObjectImpl>(initList)) {}
};