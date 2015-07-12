/*
This example illustrates the usage of std::declval.

More details here: http://en.cppreference.com/w/cpp/utility/declval
*/
#include <iostream>
#include <type_traits>
#include <utility>

int main()
{
	//Will these types be identical?
	static_assert(std::is_same<decltype(std::string()),
					decltype(std::declval<std::string>())>::value,
					"The types are different.");

	/*
	//You can view std::declval as simply calling std::add_rvalue_reference under the hood.
	//One way to fix this is to use std::remove_reference
	static_assert(std::is_same<decltype(std::string()),
					std::remove_reference<decltype(std::declval<std::string>())>::type>::value,
					"The types are different.");
	*/
	
	return 0;
}