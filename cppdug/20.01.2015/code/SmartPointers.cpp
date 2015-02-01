/*
This example aims to demonstrate the benefit of smart pointers.

In this scenario, a tree structure needs to be implemented, where the root node is unique
and it has references to Admin nodes, which also have references to Child nodes.
Also, the child nodes need to hold a reference to their parent.
*/
#include <iostream>
#include <string>
#include <deque>
#include <memory>
#include <cassert>
#include <typeinfo>

//forward declaration
class ChildNode;

//Allow Children to hold weak references to Admins
class AdminNode: public std::enable_shared_from_this<AdminNode>
{
public:
	typedef std::deque<std::shared_ptr<ChildNode>>::size_type ChildrenSizeType;

	AdminNode(const std::string &name): name_(name)
	{
		std::cout << name_ << " admin has been created!" << std::endl;
	}

	~AdminNode()
	{
		std::cout << "Destroying admin node: " << name_ << std::endl;
	}

	void AddChild(const std::string name)
	{
		//shared_from_this() returns a std::shared_ptr<AdminNode> that shares the ownership of *this
		//with all existing std::shared_ptr<AdminNode> that refer to *this
		children_.emplace_back(std::make_shared<ChildNode>(name, shared_from_this()));
	}

	std::shared_ptr<ChildNode> GetFirstChild() const
	{
		if (children_.empty())
		{
			return nullptr;
		}

		return children_.front();
	}

	ChildrenSizeType GetChildCount()
	{
		return children_.size();
	}

	const std::string &GetName()
	{
		return name_;
	}

private:
	//Admin nodes own Child nodes
	std::deque<std::shared_ptr<ChildNode>> children_;

	std::string name_;
};

class ChildNode
{
public:
	//note that we construct an std::weak_ptr from an std::shared_ptr instance
	ChildNode(const std::string &name, const std::shared_ptr<AdminNode> admin): name_(name), admin_(admin)
	{
		std::cout << name_ << " child has been created!" << std::endl;
	}

	~ChildNode()
	{
		std::cout << "Destroying child: " << name_ << std::endl;
	}

	AdminNode::ChildrenSizeType GetSiblingCount()
	{
		//Check whether the object referred to by the std::weak_ptr has already been deleted
		if (admin_.expired())
		{
			return 0;
		}

		//note the call to lock! This creates a new std::shared_ptr instance that shares ownership of the managed object.
		//std::cout << typeid(admin_.lock()).name() << std::endl;
		return admin_.lock()->GetChildCount();
	}

	const std::string &GetName()
	{
		return name_;
	}

private:
	std::string name_;

	//Child nodes are owned by Admin nodes
	std::weak_ptr<AdminNode> admin_;
};

class Root
{
public:
	Root()
	{
		std::cout << "A root node has been spawned!" << std::endl;
	}
	~Root()
	{
		std::cout << "Destroying root node!" << std::endl;
	}

	void AddAdmin(const std::string name)
	{
		admins_.emplace_back(std::make_shared<AdminNode>(name));
	}

	std::shared_ptr<AdminNode> GetFirstAdmin() const
	{
		if (admins_.empty())
		{
			return nullptr;
		}

		return admins_.front();
	}

private:
	//The root node owns Admin nodes
	std::deque<std::shared_ptr<AdminNode>> admins_;
};

//Note that && binds only to rvalues. Try calling this function on an lvalue
void DeleteRoot(std::unique_ptr<Root> &&root)
{
	//std::unique_ptr::reset() calls delete on the owned object and default initializes the std::unique_ptr instance
	root.reset();

	if (root == nullptr)
	{
		std::cout << "Yes, the root node has been disposed." << std::endl;
	}
}

int main()
{
	/*
	"When in doubt, prefer std::unique_ptr". It can later be moved to a std::shared_ptr, if needeed.
	
	Warning: In other contexts, besides being inefficient, calling new explicitly to initialize smart pointers may cause memory leaks!
	Always prefer std::make_unique and std::make_shared.
	See here for details: http://herbsutter.com/2013/05/29/gotw-89-solution-smart-pointers/
	*/
	std::unique_ptr<Root> root = std::make_unique<Root>();//they forgot to add std::make_unique in C++11...

	root->AddAdmin("Admin1");
	root->AddAdmin("Admin2");
	root->AddAdmin("Admin3");

	root->GetFirstAdmin()->AddChild("Child1");
	root->GetFirstAdmin()->AddChild("Child2");
	
	//Can anyone guess how the output will change if I uncomment this line?
	//auto firstChildOfFirstAdmin = root->GetFirstAdmin()->GetFirstChild();

	std::cout << "Number of siblings of the first child of the first admin: " << root->GetFirstAdmin()->GetFirstChild()->GetSiblingCount() << std::endl;

	//Move the root node!
	//DeleteRoot(root);//won't work!
	DeleteRoot(std::move(root));
	//the local root object now lies in a "valid but unspecified state". Calling std::move on an object documents the fact that we don't intend to reuse its value again
	//More details here: http://stackoverflow.com/questions/9168823/reusing-a-moved-container

	std::cout << "All created nodes have been automagically disposed during the DeleteRoot call." << std::endl;

	return 0;
}