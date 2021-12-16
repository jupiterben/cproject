#pragma once

#include <string>
#include "var.h"

class String : public var
{
public:
	String(const var &a);
	String(std::string &&s);
	String(std::wstring &&s);
	String(const char[]);
	String(const wchar_t[]);
	String operator+(const String &other) const;

	const static String UndefindStr;

	const std::wstring& cStr()const;
};
