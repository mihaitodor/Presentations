/*
This example illustrates the issues with range-based for loops and containers that construct proxy objects when iterating over their contents.

Details here:
http://isocpp.org/blog/2012/11/on-vectorbool
http://stackoverflow.com/questions/17794569/why-is-vectorbool-not-a-stl-container

A way to get around this issue:
http://stackoverflow.com/questions/15927033/what-is-the-correct-way-of-using-c11s-range-based-for
*/
#include <iostream>
#include <vector>

template <typename T>
void Reset(std::vector<T> &data)
{
	for (auto &item : data)
	{
		item = T();
	}
}

/*
template <typename T>
void ResetGeneric(std::vector<T> &data)
{
	for (auto &&item : data)
	{
		item = T();
	}
}
*/

int main()
{
	std::vector<int> data1 {1, 2, 3};
	
	Reset(data1);
	
	/*
	//Will this work?
	std::vector<bool> data2 {true, false, true};
	Reset(data2);
	*/
	
	/*
	ResetGeneric(data2);
	for (auto item : data2)
	{
		std::cout << item << '\n';
	}
	*/
	
	return 0;
}