
#include <jsc/js.h>

void UTest1()
{
    String s1 = _A("hello");
	console.log(s1);
	
	var s2 = _U("中国");
	Object o = {
		{s1, s2}
	};
	Array a = {1, o, s1, o, s1, o, s1};
	console.log(a);
}