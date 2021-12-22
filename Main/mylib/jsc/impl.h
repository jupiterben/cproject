#pragma once
class String;
class IImpl
{
public:
	//virtual size_t hashCode() const = 0;
	virtual ~IImpl() {}
};

class IValue : public IImpl
{
public:
	virtual String toString() const = 0;
};