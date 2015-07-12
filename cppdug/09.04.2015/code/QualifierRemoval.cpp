/*
This exercise aims to demonstrate the usage of qualifier removal utilities (std::remove_*) from the <type_traits> header.
*/
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>

template <typename T>
void Test()
{
	//remove the reference
	typename std::remove_reference<T>::type test1 = 42;
	std::cout << std::is_lvalue_reference<decltype(test1)>::value << std::endl;
	std::cout << std::is_const<decltype(test1)>::value << std::endl;
	
	//remove the reference and the const
	typename std::remove_const<typename std::remove_reference<T>::type>::type test2 = 42;
	std::cout << std::is_lvalue_reference<decltype(test2)>::value << std::endl;
	std::cout << std::is_const<decltype(test2)>::value << std::endl;
	
	/*
	//Will this also work?
	typename std::remove_reference<typename std::remove_const<T>::type>::type test3 = 42;
	std::cout << std::is_lvalue_reference<decltype(test3)>::value << std::endl;
	std::cout << std::is_const<decltype(test3)>::value << std::endl;
	*/
	
	/*
	Details: http://stackoverflow.com/questions/15887144/stdremove-const-with-const-references
	(the const qualification is not top-level)
	
	See the "Clockwise/Spiral Rule" for a good way of interpreting declarations in C (and C++):
	http://c-faq.com/decl/spiral.anderson.html
	*/
}

int main()
{
	Test<const int &>();

	return 0;
}