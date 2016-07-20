/*
See the original code for details: https://github.com/mihaitodor/Presentations/blob/master/cppdug/20.01.2015/code/TemplateFactoryWithMethodDetection.cpp

Note: The above code has several flaws which are highlited here!
*/
#include <iostream>
#include <utility>

class Object
{
public:
	Object()
	{
		std::cout << "Object constructor.\n";
	}
	//This method MUST be called after an Object instance has been constructed.
	bool Init()
	{
		std::cout << "Performing object initialization.\n";
		return true;
	}
};

class SimpleObject
{
public:
	SimpleObject()
	{
		std::cout << "Simple object constructor.\n";
	}
};

//Initialize SimpleObject (do nothing, in our case)
SimpleObject InitializeObject(SimpleObject &obj)
{
	return obj;
}

/*
int operator,(bool x, Object obj)
{
	return 0;
}
*/

//Initialize all other objects
//Avoid issues with overloaded comma operators
template <typename T>
auto InitializeObject(T &obj) -> decltype(std::declval<T>().Init(), void(), T())
{
	obj.Init();
	return obj;
}

/*
Did anyone notice the asymmetry between using std::declval<T>() in the first expression
and T() in the third expression inside decltype?

What will happen if Object::Object() is private?
(ObjectFactory can he a friend of Object, so it can call its constructor)
(Yes, C++ spports template friends)

My original code should have looked like this:

template<typename T>
auto InitializeObject(T &obj) -> std::remove_reference<decltype(std::declval<T>().Init(), void(), std::declval<T>())>::type
{
	obj.Init();
	return obj;
}
*/

/*
So, in this case, why do we even need to rely on SFINAE?
Overload resolution is enough to select the appropriate function in this simple case!

A more robust way would be to use template specialization.

template <typename T>
T InitializeObject(T &obj)
{
	obj.Init();
	return obj;
}

template <>
SimpleObject InitializeObject<SimpleObject>(SimpleObject &obj)
{
	return obj;
}
*/

template <typename T>
T ObjectFactory()
{
	T obj;
	return InitializeObject(obj);
}

int main()
{
	std::cout << "Constructing object with initializer.\n";
	auto obj = ObjectFactory<Object>();

	std::cout << "Constructing object without initializer.\n";
	auto objWithoutInitializer = ObjectFactory<SimpleObject>();
    
    return 0;
}