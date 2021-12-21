
#include <jsc/js.h>
#include "strImpl.h"
#include <windows.h>

void UTest1()
{
//  String s1 = _A("hello");
// 	console.log(s1);
	
	var s2 = _U("中国");
	StringStream ss;
	ss << s2;
	String s3 = ss.str();
// 
// 	Object o = {
// 		{s1, s2}
// 	};
// 	Array a = {1, o, s1, o, s1, o, s1};
	console.log(ss.str());
}