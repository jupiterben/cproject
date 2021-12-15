#pragma once

class NumberImpl;
class IFunctorImpl;
class StringImpl;
class ArrayImpl;
class IImpl
{
public:
	virtual IFunctorImpl *toFunctor() { return nullptr; }
	virtual NumberImpl *toNumber() { return nullptr; }
	virtual StringImpl * toString() { return nullptr; }
	virtual ArrayImpl* toArray() { return nullptr; }
};