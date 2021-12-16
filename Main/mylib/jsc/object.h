#pragma once
#include "impl.h"
#include <map>
#include "var.h"
#include "str.h"
#include <initializer_list>

typedef std::initializer_list<std::pair<const String, var>> InitialListType;
typedef std::map<String, var> InitialMapType;

class Object : public var
{
public:
	Object(const InitialMapType &keyValues) ;
	Object(InitialListType initList) ;
};