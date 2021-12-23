
#include <jsc/js.h>
#include "strImpl.h"

void UTest1()
{
	var s2 = _U("中国");
	var s1 = _U("hello");

	Object o = {
		{s1, s2}
	};
	Array a = {1, o, s1};
	Array b = { 1, o, s1 };
	if (a == b)
	{
		console.log(a);
	}
	
}