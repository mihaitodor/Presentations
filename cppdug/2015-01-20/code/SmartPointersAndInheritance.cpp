/*
This example aims to demonstrate the benefit of smart pointers and how to achieve polymorphic behaviour using such constructs.
*/
#include <iostream>
#include <memory>
#include <cassert>

class Base
{
public:
	//the virtual destructor enables RTTI: http://en.wikibooks.org/wiki/C%2B%2B_Programming/RTTI
	virtual ~Base()
	{
	}
};

class Derived : public Base
{
};

class Child : public Derived
{
public:
	Child() : data_(0)
	{
	}

	int GetData()
	{
		return data_;
	}
private:
	int data_;
};

void TestConstCast()
{
	auto constBase =  std::make_shared<const Base>();

	//auto base = std::shared_ptr<Base>(constBase);//cannot convert from const Base to Base

	//std::const_pointer_cast to the rescue!
	auto base = std::shared_ptr<Base>(std::const_pointer_cast<Base>(constBase));
	assert (base != nullptr);
}

void TestStaticCast()
{
	auto derived = std::make_shared<Derived>();

	//up-casts work implicitly
	auto base = std::shared_ptr<Base>(derived);

	//but down-casts do not work implicitly
	//auto invalidDerived = std::shared_ptr<Derived>(base);//cannot convert from Base to Derived

	//down-casts require std::static_pointer_cast
	auto sameDerived = std::static_pointer_cast<Derived>(base);

	assert(sameDerived == derived);
}

void TestDynamicCast()
{
	auto derived = std::make_shared<Derived>();

	auto base = std::shared_ptr<Base>(derived);

	//Let's assume that we don't know the polymorphic type of base
	auto badChild = std::static_pointer_cast<Child>(base);
	assert(badChild != nullptr);//well, shoot, now child points to invalid memory and no exception was thrown!
	std::cout << "Garbage data: " << badChild->GetData() << '\n';

	//std::dynamic_pointer_cast to the rescue!
	auto child = std::dynamic_pointer_cast<Child>(base);
	assert(child == nullptr);//std::dynamic_pointer_cast will place nullptr in the result, if the cast fails

	auto initialDerived = std::dynamic_pointer_cast<Derived>(base);
	assert(initialDerived != nullptr);
}

int main()
{
	TestConstCast();
	TestStaticCast();
	TestDynamicCast();

	return 0;
}