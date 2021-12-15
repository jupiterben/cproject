
#include <var.h>
#include <functor.h>

var add(var p1, var p2)
{
	return p1() + p2();
}

int main()
{
	var a = 2;
	var f = a + 2;
	var fa = Functor(add, a, f) + f + a;
	for (int i = 1; i < 10000; i++) fa = fa / fa;
	var result = fa();
	return 1;
}