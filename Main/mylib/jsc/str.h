#pragma once
#include "var.h"

class TStr;
class String : public var
{
public:
	String(const var &a);
	//String(const std::string& s);
	String(const std::u32string& s);
	String operator+(const String &other) const;
	const static String UndefinedStr;
	const static String EmptyStr;
public:
	String(const TStr& s);
	const TStr& str()const;
};


#include <sstream>
class StringStream
{
public:
	StringStream& operator<<(const String& s);
	String str()const;
private:
	std::stringstream ss;
};