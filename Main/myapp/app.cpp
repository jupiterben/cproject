
#include <jsc/var.h>
#include <jsc/functor.h>
#include <jsc/number.h>
#include <jsc/object.h>
#include <jsc/str.h>
#include <jsc/array.h>
#include <jsc/console.h>
#include <iostream>
#include <string>

int main()
{
	String s1 = _TS("hello");
	console.log(s1);
	
	var s2 = _TS("中国");
	Object o = 
	{
		{s1, s2}
	};
	Array a = {1, o, s1, o, s1, o, s1};
	console.log(a);
	return 1;
}
