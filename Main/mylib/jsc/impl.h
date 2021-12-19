#pragma once
#include <string>
class NumberImpl;
class IFunctorImpl;
class StrImpl;
class ArrayImpl;
class ObjectImpl;
#define _U(x) std::u32string(U##x)
#define _A(x) std::string(x)

class String;
class IImpl
{
public:
	//
	virtual String toString() const = 0;
};