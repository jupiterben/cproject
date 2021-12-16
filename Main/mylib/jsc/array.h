#pragma once
#include <initializer_list>
#include "var.h"

class Array : public var
{
public:
	Array(const var& other);
	Array(std::initializer_list<var> alist);
};