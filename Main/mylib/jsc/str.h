#pragma once
#include "var.h"
#include <string>
#define _U(x) std::u32string(U##x)

class TStr;
class String : public var
{
public:
	String(const var &a);
	// String(const std::string& s);
	String(const std::u32string &s);
	String operator+(const String &other) const;
	const static String UndefinedStr;
	const static String EmptyStr;

public:
	String(const TStr &s);
	const TStr &str() const;
};

class StringStream : public var
{
public:
	StringStream();
	StringStream &operator<<(const std::u32string& s);
	StringStream &operator<<(const String &s);
	String str() const;
};