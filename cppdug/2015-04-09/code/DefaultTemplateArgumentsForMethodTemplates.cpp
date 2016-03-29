/*
This exercise demonstrates the usage of default template arguments for method templates.
*/
#include <iostream>

template <typename T>
class Object
{
public:
	Object(T data) : data_(data)
	{
	}
	
	template<typename U = T>
	U MultiplyBy(T input)
	{
		return data_ * input;
	}
	
private:
	T data_;
};

int main()
{
	Object<double> obj(3.14);
	
	std::cout << obj.MultiplyBy<int>(2) << std::endl;
	
	/*
	In this case, the return type of MultiplyBy cannot be deduced,
	thus it falls back to the default template argument
	*/
	std::cout << obj.MultiplyBy(2) << std::endl;
	
	return 0;
}