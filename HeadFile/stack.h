#ifndef  JIUGESTL_HEADFILE_STACK
#define  JIUGESTL_HEADFILE_STACK

#include "deque.h"

// 这个头文件包含了一个模板类 stack
namespace JStl{

template <typename T, class Container = JStl::deque<T>>
class stack
{
public:
	//底层容器
	typedef Container                           container_type;
	typedef typename Container::value_type      value_type;
	typedef typename Container::size_type       size_type;
	typedef typename Container::reference       reference;
	typedef typename Container::const_reference const_reference;

	static_assert(std::is_same<T, value_type>::value,
		"the value_type of Container should be same with T");
private:
	container_type c_;  

public:
	//构造，拷贝构造，移动构造，析构，拷贝赋值，移动赋值
	stack() = default;

	stack(size_type n)
		:c_(n)
	{}

	stack(size_type n, const value_type& value)
		:c_(n, value)
	{}

	template <class Iter>
	stack(Iter first, Iter last)
		: c_(first, last)
	{}

	stack(std::initializer_list<T> l)
		:c_(l.begin(), l.end())
	{}

	stack(const Container& c)
		:c_(c)
	{}

	stack(Container&& c) 
		: c_(JStl::move(c))
	{}

	stack(const stack& rhs)
		:c_(rhs.c_)
	{}

	stack(stack&& rhs) 
		: c_(JStl::move(rhs.c_))
	{}

	stack& operator=(const stack& rhs)
	{
		c_ = rhs.c_;
		return *this;
	}

	stack& operator=(stack&& rhs) 
	{
		c_ = JStl::move(rhs.c_);
		return *this;
	}

	stack& operator=(std::initializer_list<T> l)
	{
		c_ = l;
		return *this;
	}

	~stack() = default;

public:
	reference top()       
	{ 
		return c_.back(); 
	}

	const_reference top() const 
	{
		return c_.back(); 
	}

	bool empty() const
	{ 
		return c_.empty(); 
	}

	size_type size() const
	{ 
		return c_.size(); 
	}

public:
	template<typename... Args>
	void emplace_back(Args&& ...args)
	{
		c_.emplace_back(JStl::forward<Args>(args)...);
	}

	void push(const value_type &v)
	{
		c_.push_back(v);
	}

	void push(value_type &&v)
	{
		c_.push_back(JStl::move(v));
	}

	void pop()
	{
		c_.pop_back();
	}

	void clear()
	{
		while (!empty())
			pop();
	}

	void swap(stack& rhs) 
	{
		JStl::swap(c_, rhs.c_);
	}

	friend bool operator==(const stack& lhs, const stack& rhs) 
	{ 
		return lhs.c_ == rhs.c_; 
	}

	friend bool operator<(const stack& lhs, const stack& rhs) 
	{ 
		return lhs.c_ <  rhs.c_; 
	}
};

template <class T, class Container>
bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
	return !(lhs == rhs);
}

template <class T, class Container>
bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
	return rhs < lhs;
}

template <class T, class Container>
bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
	return !(rhs < lhs);
}

template <class T, class Container>
bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
	return !(lhs < rhs);
}

template <class T, class Container>
void swap(stack<T, Container>& lhs, stack<T, Container>& rhs)
{
	lhs.swap(rhs);
}

}//namesapce JStl
#endif