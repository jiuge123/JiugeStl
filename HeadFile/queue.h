#ifndef  JIUGESTL_HEADFILE_QUEUE
#define  JIUGESTL_HEADFILE_QUEUE

#include "deque.h"

// queue          : 队列
// priority_queue : 优先队列

namespace JStl{
template <typename T, typename Container = JStl::deque<T>>
class queue
{
public:
	typedef Container                           container_type;

	typedef typename Container::value_type      value_type;
	typedef typename Container::size_type       size_type;
	typedef typename Container::reference       reference;
	typedef typename Container::const_reference const_reference;

	static_assert(std::is_same<T, value_type>::value,
		"the value_type of Container should be same with T");
private:
	container_type c_;  // 用底层容器表现 queue

public:
	//构造，拷贝构造，移动构造，析构，拷贝赋值，移动赋值

	queue() = default;

	queue(size_type n)
		:c_(n)
	{}

	queue(size_type n, const value_type& value)
		:c_(n, value)
	{}

	template <class IIter>
	queue(IIter first, IIter last)
		: c_(first, last)
	{}

	queue(std::initializer_list<T> l)
		:c_(l.begin(), l.end())
	{}

	queue(const Container& c)
		:c_(c)
	{}

	queue(Container&& c)
		: c_(JStl::move(c))
	{}

	queue(const queue& rhs)
		:c_(rhs.c_)
	{}

	queue(queue&& rhs)
		: c_(JStl::move(rhs.c_))
	{}

	queue& operator=(const queue& rhs)
	{
		c_ = rhs.c_;
		return *this;
	}

	queue& operator=(queue&& rhs)
	{
		c_ = JStl::move(rhs.c_);
		return *this;
	}

	queue& operator=(std::initializer_list<T> l)
	{
		c_ = l;
		return *this;
	}

	~queue() = default;

public:
	reference front()      
	{ 
		return c_.front(); 
	}

	const_reference front() const 
	{ 
		return c_.front(); 
	}

	reference back()        
	{ 
		return c_.back();
	}

	const_reference back()  const 
	{ 
		return c_.back(); 
	}

	bool empty() const 
	{ 
		return c_.empty(); 
	}

	size_type size()  const 
	{ 
		return c_.size(); 
	}

	template <class ...Args>
	void emplace(Args&& ...args)
	{
		c_.emplace_back(JStl::forward<Args>(args)...);
	}

	void push(const value_type& value)
	{
		c_.push_back(value);
	}

	void push(value_type&& value)
	{
		c_.emplace_back(JStl::move(value));
	}

	void pop()
	{
		c_.pop_front();
	}

	void clear()
	{
		while (!empty())
			pop();
	}

	void swap(queue& rhs)
	{
		JStl::swap(c_, rhs.c_);
	}

public:
	friend bool operator==(const queue& lhs, const queue& rhs) 
	{ 
		return lhs.c_ == rhs.c_; 
	}

	friend bool operator< (const queue& lhs, const queue& rhs)
	{ 
		return lhs.c_ < rhs.c_; 
	}
};


template <class T, class Container>
bool operator!=(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
{
	return !(lhs == rhs);
}

template <class T, class Container>
bool operator>(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
{
	return rhs < lhs;
}

template <class T, class Container>
bool operator<=(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
{
	return !(rhs < lhs);
}

template <class T, class Container>
bool operator>=(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
{
	return !(lhs < rhs);
}

template <class T, class Container>
void swap(queue<T, Container>& lhs, queue<T, Container>& rhs)
{
	lhs.swap(rhs);
}

/****************************************************************************/
//priority_queue
//默认使用less作为比较对象
template<typename T, typename Container = JStl::vector<T>, 
	typename Compare = JStl::less<typename Container::value_type >>
class priority_queue{
public:
	typedef Container                           container_type;
	typedef Compare                             value_compare;

	typedef typename Container::value_type      value_type;
	typedef typename Container::size_type       size_type;
	typedef typename Container::reference       reference;
	typedef typename Container::const_reference const_reference;

	static_assert(std::is_same<T, value_type>::value,
		"the value_type of Container should be same with T");

private:
	container_type c_;     // 底层容器
	value_compare  comp_;  // 比较标准

};

}//namesapce JStl
#endif