#pragma once
#include <stddef.h>
class String;
class IImpl
{
public:
	virtual ~IImpl() {}
};

class IValue : public IImpl
{
public:
	virtual String toString() const = 0;
	virtual size_t getHash() const = 0;
};