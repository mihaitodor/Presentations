/*
This example illustrates probably the only legit usage of const_cast: code reuse.

Details here: http://stackoverflow.com/questions/123758/how-do-i-remove-code-duplication-between-similar-const-and-non-const-member-func
*/

#include <iostream>
#include <string>

class Object
{
public:
	Object(const std::string &name): name_(name)
	{
	}
	
	const std::string &Test() const
	{
		std::cout << name_ << '\n';
		return name_;
	}

	//Here we want to reuse the code from the const overload of Test().
	std::string &Test()
	{
		//Will this work?
		return this->Test();

		/*
		Let's try adding const to "this".
		Use static_cast instead of const_cast, which is intended only to "cast away (remove) constness or volatility", not add it.
		http://en.cppreference.com/w/cpp/language/const_cast
		*/
		/*
		return static_cast<const Object *>(this)->Test();
		*/
		/*
		return const_cast<std::string &>(static_cast<const Object *>(this)->Test());
		*/
	}
	
private:
	std::string name_;
};

int main()
{
	Object obj("Object");
	const Object constObj("Const Object");

	obj.Test();
	constObj.Test();
	
	return 0;
}