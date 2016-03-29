/*
In this example, I will illustrate a simple template metaprogramming technique, which enables conditional object initialization.

Basically, the template overload of InitializeObject() will be called on a new instance of T, but if T is an instance of SimpleObject, then the non-template overload of InitializeObject() will be called.

Note: Although the constructors should be private for a proper Factory implementation,
I am leaving them public in order to not complicate the example with template function forward declarations
*/
#include <iostream>

class Object
{
public:
	Object()
	{
		std::cout << "Object constructor." << std::endl;
	}
	//This method MUST be called after an Object instance has been constructed.
	void Init()
	{
		std::cout << "Performing object initialization." << std::endl;
	}
};

class SimpleObject
{
public:
	SimpleObject()
	{
		std::cout << "Simple object constructor." << std::endl;
	}
};

//Initialize SimpleObject (do nothing, in our case)
SimpleObject InitializeObject(std::true_type, SimpleObject &obj)
{
	return obj;
}

//Initialize all other objects
template <typename T>
T InitializeObject(std::false_type, T &obj)
{
	obj.Init();
	return obj;
}

template <typename T>
T ObjectFactory()
{
	T obj;
	return InitializeObject(std::is_same<T, SimpleObject>(), obj);
}

int main()
{
	std::cout << "Constructing object with initializer." << std::endl;
	auto obj = ObjectFactory<Object>();

	std::cout << "Constructing object without initializer." << std::endl;
	auto objWithoutInitializer = ObjectFactory<SimpleObject>();
    
    return 0;
}