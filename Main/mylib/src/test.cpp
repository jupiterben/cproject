
#include <jsc/js.h>

var testF(var a, var b)
{
	return a;
}

void UTest2()
{
	var f = Functor(testF, 1, 1);
	var f2 = Functor(testF, 1, 2);
	f();
	f2();
}

void UTest1()
{
	Number a = 1;
	var b = 2;
	var c = a + b;

	var f = Functor(testF, a, b);
	console.log(f());

	var s = Switch(a)
			.Case(1, 3)
			.Case(2, 3)
			.Default(4);

	
			
	console.log(s());
}