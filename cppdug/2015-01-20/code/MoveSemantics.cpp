/*
This example aims to demonstrate the power of move semantics.

For an in depth explanation, see this blog post:
http://blogs.msdn.com/b/vcblog/archive/2009/02/03/rvalue-references-c-0x-features-in-vc10-part-2.aspx
THe above link seems broken now (truncated content). The original material can be found here: web.archive.org/web/20151121050000/http://blogs.msdn.com/b/vcblog/archive/2009/02/03/rvalue-references-c-0x-features-in-vc10-part-2.aspx

More reading material about RVO and NRVO:
http://blog.knatten.org/2011/08/26/dont-be-afraid-of-returning-by-value-know-the-return-value-optimization/
http://definedbehavior.blogspot.nl/2011/08/value-semantics-nrvo.html
http://definedbehavior.blogspot.ch/2011/08/value-semantics-copy-elision.html
http://juanchopanzacpp.wordpress.com/2014/05/11/want-speed-dont-always-pass-by-value/
*/
#include <iostream>
#include <vector>

class Object
{
public:
	Object()
	{
		std::cout << "Object constructor\n";
	}
	
	Object(const Object &obj)
	{
		std::cout << "Oh noes, a copy has been made!\n";
	}

	Object &operator=(const Object &obj)
	{
		std::cout << "Oh noes, a copy assignment has been made!\n";
		return *this;
	}

	//After this call, the input, obj, will end up in  an unspecified, but valid state
	Object(Object &&obj)
	{
		std::cout << "A move has been made!\n";
	}

	Object &operator=(Object &&obj)
	{
		std::cout << "A move assignment has been made!\n";
		return *this;
	}
};

//RVO
Object CreateObject()
{
	return Object();
}

//NRVO
Object CreateAndCopyObject()
{
	Object obj;
	
	return obj;
}

int main()
{
	Object obj1 = CreateObject();
	Object obj2 = CreateAndCopyObject();
	
	/*
	std::vector<Object> data(2);
	
	//After this step, the contents of the data vector lie in an unspecified, but valid state.
	//Since you moved them, you should remember to reinitialize them.
	std::vector<Object> movedData(std::make_move_iterator(data.begin()),
									std::make_move_iterator(data.end()));
									
	//					named rvalue		xvalue
	//					reference
	std::vector<Object> &&movedVector = std::move(movedData);
	*/

	return 0;
}
