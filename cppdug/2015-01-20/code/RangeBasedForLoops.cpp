/*
This example aims to demonstrate the benefit of range-based for loops and other possible ways in which to iterate over STL container items.
*/
#include <iostream>
#include <iterator>
#include <vector>

class Object
{
public:
	Object()
	{
		std::cout << "Constructor" << std::endl;
	}
	
	Object(const Object &input)
	{
		std::cout << "Oh noes, a copy has been made!" << std::endl;
	}
	
	
	Object(Object &&input)
	{
		std::cout << "Move constructor" << std::endl;
	}

	void Test()
	{
		std::cout << "Test" << std::endl;
	}

	void Test() const
	{
		std::cout << "Const test" << std::endl;
	}
};

int main()
{
	std::vector<Object> objectVector(1);

	//Use a simple Range-based for loop to iterate over a vector of custom objects in order:
	
	//Will this work properly?
	for (auto value : objectVector)
	{
		value.Test();
	}

	//Iterate backwards:
	
	std::vector<int> intVector;
	intVector.push_back(1);
	intVector.push_back(2);

	for (auto it = intVector.rbegin(); it != intVector.rend(); ++it)
	{
		std::cout << *it << std::endl;
	}
	
	//Or, if we have the regular bidirectional iterators (C++14):
	/*
	//Looks like this GCC version doesn't have std::make_reverse_iterator :(
	for (auto it = make_reverse_iterator(intVector.end()); it != make_reverse_iterator(intVector.begin()); ++it)
	{
		std::cout << *it << std::endl;
	}
	*/
	
	/*
	//A Boost adaptor is required in order to achieve reverse iteration with range based for loops
	//http://www.boost.org/doc/libs/1_55_0/libs/range/doc/html/range/reference/adaptors/reference/reversed.html
	//for (const auto &value : boost::adaptors::reverse(intVector))
	//{
	//	std::cout << value << std::endl;
	//}
	*/
	
	return 0;
}