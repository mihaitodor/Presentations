/*
This example aims to demonstrate the usage of lambdas.

[=] ( ) mutable throw() -> int { return 42; }
--- --- ------- ------- ------ --------------
 1   2     3       4       5         6

1. lambda-introducer (Also known as the capture clause)
2. lambda declarator (Also known as the parameter list). Optional!
3. mutable (Also known as the mutable specification). Optional!
4. exception-specification (Also known as the exception specification). Optional!
5. trailing-return-type (Also known as the return type). Optional!
6. compound-statement (Also known as the lambda body)
*/
#include <iostream>
#include <string>
#include <cstdint>
#include <cassert>
#include <functional>

void CaptureNothing()
{
	int x = 0;

	[]
	{
		//Will this work?
		//int y = x;
	}();
}

void CaptureAll()
{
	int x = 42;

	std::cout << "Capture all by value:\n";
	//by default, variables captured by value are non-modifiable, so the mutable specification is required
	[=] () mutable
	{
		x = 3;
	}();

	std::cout << x << '\n';

	std::cout << "Capture all by reference:\n";
	[&]
	{
		x = 3;
	}();

	std::cout << "x: " << x << '\n';
}

void SelectiveCaptureMixed()
{
	int x = 1;
	int y = 2;
	int z = 3;

	std::cout << "Capture x by value and y by reference:\n";
	[x, &y] () mutable
	{
		y = x;
		x = 42;
		//Will this work?
		//z = 42;
	}();

	std::cout << "x: " << x << '\n';
	std::cout << "y: " << y << '\n';
}

void AutomaticReturnTypeDeduction()
{
	auto value = []{ return 42; }();

	std::cout << "Deduced type is " << typeid(value).name() << '\n';
}

void RecursiveLambda()
{
	/*
	- the auto keyword infers its type from whatever it's initialized with, but, in this case,
		the expression that we are using to initialize it with depends on the type, so we need to break this circular dependency by specifying the type explicitly
	- note that we capture the functor object in the lambda-introducer by reference
	*/
	std::function<uint64_t(uint64_t)> fibonacci = [&fibonacci](uint64_t n) { return n < 2 ? n : fibonacci(n - 1) + fibonacci(n - 2); };

	assert(fibonacci(1) == 1);
	assert(fibonacci(2) == 1);
	assert(fibonacci(3) == 2);
	assert(fibonacci(4) == 3);
	assert(fibonacci(5) == 5);
}

int main()
{
	CaptureNothing();
	CaptureAll();
	SelectiveCaptureMixed();
	AutomaticReturnTypeDeduction();
	
	RecursiveLambda();
	
	return 0;
}