/*
This example aims to demonstrate the benefit of the nullptr keyword and the reason why it has been introduced.
*/
#include <iostream>

void Test(int *input)
{
	std::cout << "Pointer to integer received" << std::endl; 
}

void Test(int input)
{
	std::cout << "Integer received" << std::endl; 
}

int main()
{
	Test(0);
	Test(nullptr);
	
	//Will this work?
	//Test(NULL);
	
	return 0;
}