#pragma once

#include <string>
#include "var.h"

class String : public var
{
public:
	String(const var& a);
	String(const std::string& s);
	String(const std::wstring& s);
	String operator + (const String& other)const;
};
