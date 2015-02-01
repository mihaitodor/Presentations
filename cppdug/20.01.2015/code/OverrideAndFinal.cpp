/*
This example aims to demonstrate the benefit of the override and final keywords.
*/
#include <iostream>

class Base
{
public:
	virtual void OverrideMe()
	{
		std::cout << "Base" << std::endl;
	}
};

class DerivedBase : public Base
{
public:
	void OverrideMe() final override//virtual specifier can be omitted
	{
		std::cout << "Derived base" << std::endl;
	}
};

class Derived final : public DerivedBase
{
public:
	//virtual void OverrideMe();// can't override final function
};

//class DerivedDerived : public Derived//cannot inherit from 'Derived' as it has been declared as 'final'
//{
//};

class BadDerived : public Base
{
public:
	virtual void OverrideMe() const //override
	{
		std::cout << "Bad derived" << std::endl;
	}
};

int main()
{
	Base *object = new DerivedBase();

	object->OverrideMe();

	Base *badObject = new BadDerived();

	badObject->OverrideMe();//oups...
	
	return 0;
}