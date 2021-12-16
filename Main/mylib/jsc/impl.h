#pragma once

#include <string>
class NumberImpl;
class IFunctorImpl;
class StrImpl;
class ArrayImpl;
class ObjectImpl;

class IImpl
{
public:
	virtual IFunctorImpl *toFunctorImpl() { return nullptr; }
	virtual NumberImpl *toNumberImpl() { return nullptr; }
	virtual StrImpl * toStrImpl() { return nullptr; }
	virtual ArrayImpl* toArrayImpl() { return nullptr; }
	virtual ObjectImpl* toObjectImpl(){ return nullptr; }
	static IImpl* cast(IImpl* impl) { return impl; }
	//
	virtual std::wstring toString() const = 0;
};