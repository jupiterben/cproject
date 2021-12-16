
#include <jsc/var.h>
#include <jsc/functor.h>
#include <jsc/number.h>
#include <jsc/object.h>
#include <jsc/str.h>
#include <jsc/array.h>
#include <jsc/console.h>

int main()
{
	String s1 = L"hello";
	console.log(s1);
	var s2 = L"hello world";
	for (int i = 0; i < 100; i++)
	{
		s1 = s1 + s2;
	}
	Object o = 
	{
		{s1, s2}
	};
	Array a = { 1, o };
	console.log(a);
	return 1;
}