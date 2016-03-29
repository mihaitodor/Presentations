/*
This example aims to demonstrate the benefit of specializing the non-member begin() and end() functions for a custom type.

Code inspired from this tutorial: http://mbevin.wordpress.com/2012/11/14/range-based-for/

Please note that more work is required in order to fix the issue highlighted in the comments from main()...
Read the full tutorial for solutions.
*/
#include <iostream>
#include <memory>

class Node
{
public:

	Node (int data, std::shared_ptr<Node> next) : m_data(data), m_next(next)
	{
	}

	Node (const Node &input) : m_data(input.m_data), m_next(input.m_next)
	{
		std::cout << "Oh noes, a copy has been made!" << std::endl;
	}

	std::shared_ptr<Node> GetNext() const
	{
		return m_next;
	}

	void PrintData()
	{
		std::cout << m_data << std::endl;
	}

private:

	int m_data;
	std::shared_ptr<Node> m_next;
};

class Stack
{
public:
	class Iterator
	{
	public:
		Iterator(std::shared_ptr<Node> node) : m_node(node)
		{
		}

		//not equals operator
		bool operator!=(const Iterator &input) const
		{
			return m_node != input.m_node;
		}

		//prefix increment operator
		Iterator &operator++()
		{
			if (m_node != nullptr)
			{
				m_node = m_node->GetNext();
			}

			return *this;
		}
		
		//indirection operator
		Node &operator*() const
		{
			return *m_node;
		}

	private:
		std::shared_ptr<Node> m_node;
	};

	Stack () : m_top(nullptr)
	{
	}

	void Push(int data)
	{
		m_top = std::make_shared<Node>(data, m_top);
	}

	std::shared_ptr<Node> Pop()
	{
		if (!m_top) return nullptr;

		auto result = m_top;

		m_top = m_top->GetNext();

		return result;
	}

	Iterator Begin() const
	{
		return Iterator(m_top);
	}

	Iterator End() const
	{
		return Iterator(nullptr);
	}

private:
	std::shared_ptr<Node> m_top;
};

Stack::Iterator begin(const Stack &stack)
{
	return stack.Begin();
}

Stack::Iterator end(const Stack &stack)
{
	return stack.End();
}

int main()
{
	Stack stack;
	stack.Push(1);
	stack.Push(2);
	stack.Push(3);

	std::cout << "Pop the first element" << std::endl;
	stack.Pop()->PrintData();

	std::cout << "Walk the stack" << std::endl;
	for (auto &node : stack)
	{
		node.PrintData();//call mutable function OK
	}

	const auto &constStack = stack;

	std::cout << "Walk the stack via const reference" << std::endl;
	for (auto &node : constStack)
	{
		node.PrintData();//call mutable function still OK?...
	}

	return 0;
}