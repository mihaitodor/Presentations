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
	//Will this work?
	typename std::remove_reference<T>::type test1 = 42;
	std::cout << std::is_lvalue_reference<decltype(test1)>::value << '\n';
	std::cout << std::is_const<decltype(test1)>::value << '\n';

	/*
	typename std::remove_const<typename std::remove_reference<T>::type>::type test2 = 42;
	std::cout << std::is_lvalue_reference<decltype(test2)>::value << '\n';
	std::cout << std::is_const<decltype(test2)>::value << '\n';
	
	//Will this also work?
	// typename std::remove_reference<typename std::remove_const<T>::type>::type test3 = 42;
	// std::cout << std::is_lvalue_reference<decltype(test3)>::value << '\n';
	// std::cout << std::is_const<decltype(test3)>::value << '\n';
	*/
	/*
	Details: http://stackoverflow.com/questions/15887144/stdremove-const-with-const-references
	(the const qualification is not top-level)
	*/
}

int main()
{
	Test<const int &>();

	return 0;
}