/*
This example aims to demonstrate the benefit of having return type deduction and the C++11 style trailing return type.
*/
#include <iostream>
#include <typeinfo>

class A
{
public:
	int GetData()
	{
		return 0;
	}
};

class B
{
public:
	float GetData()
	{
		return 0.0;
	}
};

/*
Note that I am using the new-style return type declaration for this function, which is implied by the auto keyword.
The "->" hints that the expression following it will yield the return type.
In our case, decltype specifies that the return type of ExtractData will be deduced from the following expression: decltype(input.GetData())
decltype will yield the return type of GetData(), which will become the return type of ExtractData

C++14 provides automatic return type deduction, in which case, the return type of ExtractData will be inferred from the return and we can remove the decltype
*/
template <typename T>
auto ExtractData(T &input) /*-> decltype(input.GetData())*/
{
	return input.GetData();
}

int main()
{
	A a;
	auto aData = ExtractData(a);//aData -> int
	std::cout << typeid(aData).name() << '\n';

	B b;
	auto bData = ExtractData(b);//bData -> float
	std::cout << typeid(bData).name() << '\n';
	
	return 0;
}