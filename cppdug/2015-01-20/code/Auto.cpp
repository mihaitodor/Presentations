/*
This example aims to demonstrate the benefit of the auto keyword as well as some pitfalls.
*/
#include <iostream>
#include <vector>
#include <typeinfo>

class Object
{
public:
	void Test()
	{
		std::cout << "Test\n";
	}

	void Test() const
	{
		std::cout << "Const Test\n";
	}
};

int main()
{
	std::vector<Object> data(1);

	for (auto it = data.begin(); it != data.end(); ++it)//auto = std::vector<Object>::iterator
	{
		it->Test();
	}

	//implicitly casts std::vector<Object>::iterator to std::vector<Object>::const_iterator
	for (std::vector<Object>::const_iterator it = data.begin(); it != data.end(); ++it)
	{
		it->Test();
	}

	for (auto it = data.cbegin(); it != data.cend(); ++it)//auto = std::vector<Object>::const_iterator
	{
		it->Test();
	}

	//will this work?...
	
	// for (const auto it = data.begin(); it != data.end(); ++it)
	// {
		// it->Test();
	// }

	const auto &constData = data;

	for (auto it = constData.begin(); it != constData.end(); ++it)//auto = std::vector<Object>::const_iterator
	{
		it->Test();
	}

	//Abuse example: std::vector::size() returns std::vector::size_type, not int! Same story for all STL containers.
	std::cout << typeid(data.size()).name() << '\n';
	for (auto i = 0; i < data.size(); ++i)
	{
		std::cout << typeid(i).name() << '\n';
	}
	
	return 0;
}