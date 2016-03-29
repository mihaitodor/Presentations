/*
Please read the TemplateFactoryWithTypeDetection example first!

In this example, I will illustrate a more advanced conditional object initialization technique.

The code requires the "Expression SFINAE" feature, which has been added recently to most compilers (I think).

This code will attempt to detect if the object which needs to be initialized has a method called "Init" and, if yes, execute it.

The implementation was inspired from the answer provided for one of my Stack Overflow questions:
http://stackoverflow.com/questions/24248817/disable-method-override-in-template-derived-class
Praetorian offers a thorough explanation of this technique over here:
http://stackoverflow.com/questions/23987925/using-sfinae-to-select-function-based-on-whether-a-particular-overload-of-a-func

Note: Although the constructors should be private for a proper Factory implementation,
I am leaving them public in order to not complicate the example with template function forward declarations
*/
#include <iostream>
#include <utility>

class Object
{
public:
	Object()
	{
		std::cout << "Object constructor." << std::endl;
	}
	//This method MUST be called after an Object instance has been constructed.
	bool Init()
	{
		std::cout << "Performing object initialization." << std::endl;
		return true;
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
SimpleObject InitializeObject(SimpleObject &obj)
{
	return obj;
}

/*
Initialize all other objects
Note that I am using the new-style return type declaration for this function, which is implied by the auto keyword.
The "->" hints that the expression following it will yield the return type.
*/
//Will this work all the time?
template<typename T>
auto InitializeObject(T &obj) -> decltype(std::declval<T>().Init(), T())
{
	obj.Init();
	return obj;
}

int operator,(bool x, Object obj)
{
	return 0;
}

/*
In our case, decltype specifies that the return type of InitializeObject will be deduced from the following expression: std::declval<T>().Init(), void(), T()

If std::declval<T>().Init() is not well formed (which happens when T::Init() does not exist),
then SFINAE kicks in and this function overload is removed from the list of possible candidates.

The first sub-expression, std::declval<T>().Init(), is required to detect if T::Init() exists.
We have to use std::declval, because the expression needs to be valid (what would happen if T's default constructor is private if we don't use declval?)
Note that std::declval can only be used in unevaluated expressions.

The middle sub-expression, void() doesn't do anything other than ensure that some user-defined comma operator (operator,)
is not selected (because you cannot overload the comma operator with a void parameter type).

The third, and rightmost, sub-expression, T() is what determines the output of decltype, and, implicitly, the return type of InitializeObject.
*/
/*
template<typename T>
auto InitializeObject(T &obj) -> decltype(std::declval<T>().Init(), void(), T())
{
	obj.Init();
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
	std::cout << "Constructing object with initializer." << std::endl;
	auto obj = ObjectFactory<Object>();

	std::cout << "Constructing object without initializer." << std::endl;
	auto objWithoutInitializer = ObjectFactory<SimpleObject>();
    
    return 0;
}