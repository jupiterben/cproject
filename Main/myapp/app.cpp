
#include <jsc/var.h>
#include <jsc/functor.h>
#include <jsc/number.h>
#include <jsc/object.h>
#include <jsc/str.h>
#include <jsc/array.h>

int main()
{
	String s1 = L"hello";
	var s2 = L"hello world";
	for (int i = 0; i < 100000; i++)
	{
		s1 = s1 + s2;
	}
	Object o = 
	{
		{s1, s2}
	};
	Array a = { 1, o };
	return 1;
}