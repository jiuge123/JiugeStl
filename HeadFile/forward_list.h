#ifndef  JIUGESTL_HEADFILE_FORWARDLIST
#define  JIUGESTL_HEADFILE_FORWARDLIST

#include <cassert>

#include <initializer_list>

#include "iterator.h"
#include "uninitialized.h"
#include "allocator.h"
#include "util.h"

//包含forwara_list 单向链表

namespace JStl{

struct _flist_node_base
{
	_flist_node_base *next;
};

template<class T>
struct _flist_node:public _flist_node_base
{
	_flist_node(T x) :data(x)
	{}

	T data;
};

_flist_node_base* _flist_make_link(_flist_node_base *prev, _flist_node_base *newnode)
{
	newnode->next = prev->next;
	prev->next = newnode;
	return newnode;
}

size_t _slist_size(_flist_node_base *node)
{
	size_t n = 0;
	for (; node != nullptr; node = node->next)
		++n;
	return n;
}

struct _flist_iterator_base:public forward_iterator_tag
{
	typedef size_t					size_type;
	typedef ptrdiff_t				difference_type;
	typedef forward_iterator_tag	iterator_category;

	_flist_node_base *node_;

	_flist_iterator_base(_flist_node_base *x) :node_(x)
	{}
	
	void incr()
	{
		node_ = node_->next;
	}

	bool operator==(const _flist_iterator_base& x) const{
		return node_ == x.node_;
	}

	bool operator!=(const _flist_iterator_base& x) const{
		return !(node_ == x.node_);
	}
};

template<typename T, typename Ref, typename Ptr>
struct flist_iterator :public _flist_iterator_base
{
	typedef flist_iterator<T, T&, T*>				iterator;
	typedef flist_iterator<T, const T&, const T*>	const_iterator;
	typedef flist_iterator							self;

	typedef T				value_type;
	typedef Ptr				pointer;
	typedef Ref				reference;
	typedef _flist_node<T>  flist_node;

	flist_node *node_;

	flist_iterator() :_flist_iterator_base(nullptr)
	{}

	flist_iterator(flist_node *x) :_flist_iterator_base(x)
	{}

	flist_iterator(const flist_iterator<T, T&, T*> *x) :_flist_iterator_base(x.node)
	{}

	reference operator*() const
	{
		return node_->data;
	}

	pointer operator->() const
	{
		return &(operator*());
	}

	self& operator++()
	{
		assert(node_ != nullptr);
		incr();
		return *this;
	}

	self operator++(int)
	{
		self tmp = *this;
		incr();
		return tmp;
	}
};

template<typename T, typename Alloc = allocator<T>>
class forward_list{
public:
	typedef JStl::allocator<T>							data_allocator;
	typedef JStl::allocator<_flist_node<T>>				node_allocator;

	typedef typename allocator<T>::value_type			value_type;
	typedef typename allocator<T>::pointer				pointer;
	typedef typename allocator<T>::const_pointer		const_pointer;
	typedef typename allocator<T>::reference			reference;
	typedef typename allocator<T>::const_reference		const_reference;
	typedef typename allocator<T>::size_type			size_type;
	typedef typename allocator<T>::difference_type		difference_type;

	typedef JStl::flist_iterator<T,T&,T*>				iterator;
	typedef JStl::flist_iterator<T,const T&,const T*>   const_iterator;
														
	typedef	_flist_node_base			flist_node_base;
	typedef	_flist_node<T>				flist_node;
	typedef _flist_node<T>*				node_ptr;
	typedef	_flist_iterator_base		iterator_base;

private:
	flist_node_base head_;

private:
	template<class... Args>
	node_ptr create_node(Args&& ...args);

	void destroy_node(node_ptr node);

public:
	//构造，拷贝构造，移动构造，析构，拷贝赋值，移动赋值
	forward_list();

	forward_list(size_type n);

	forward_list(size_type n, const value_type& v);

	template<typename Iter, typename std::enable_if<
		JStl::is_input_iterator<Iter>::value, int>::type = 0>
	forward_list(Iter first, Iter last);

	forward_list(std::initializer_list<T> l);

	forward_list(const forward_list& rhs);

	forward_list(forward_list &&rhs);

	forward_list& operator=(const forward_list& rhs);

	forward_list& operator=(forward_list &&rhs);

	forward_list& operator=(std::initializer_list<T> l);

	~forward_list();
public:
	//迭代器相关操作
	iterator begin()
	{
		return iterator((node_ptr)head_.next);
	}

	const_iterator begin() const
	{
		return iterator((node_ptr)head_.next);
	}

	iterator end()
	{
		return iterator();
	}

	const_iterator end() const
	{
		return iterator();
	}

	const_iterator cbegin() const
	{
		return begin();
	}

	const_iterator cend() const
	{
		return end();
	}

};

template<typename T, typename Alloc = allocator<T>>
template<class... Args>
typename forward_list<T, Alloc>::node_ptr
forward_list<T, Alloc>::create_node(Args&& ...args)
{
	node_ptr node = node_allocator::allocate();
	node_allocator::construct(node, JStl::forward<Args>(args)...);
	node->next = nullptr;
	return node;
}

template<typename T, typename Alloc = allocator<T>>
void forward_list<T, Alloc>::destroy_node(node_ptr node)
{
	node_allocator::destroy(node);
	node_allocator::deallocate(node);
}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>::forward_list()
{
	head_.next = nullptr;
}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>::forward_list(size_type n)
{
	auto p = &head_;
	while (n--){
		auto node = create_node(value_type());
		_flist_make_link(p, node);
		p = p->next;
	}
}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>::forward_list(size_type n, const value_type& v)
{

}

template<typename T, typename Alloc = allocator<T>>
template<typename Iter, typename std::enable_if<
	JStl::is_input_iterator<Iter>::value, int>::type = 0>
forward_list<T, Alloc>::forward_list(Iter first, Iter last)
{

}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>::forward_list(std::initializer_list<T> l)
{

}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>::forward_list(const forward_list& rhs)
{

}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>::forward_list(forward_list &&rhs)
{

}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>& forward_list<T, Alloc>::operator=(const forward_list& rhs)
{

}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>& forward_list<T, Alloc>::operator=(forward_list &&rhs)
{

}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>& forward_list<T, Alloc>::operator=(std::initializer_list<T> l)
{

}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>::~forward_list()
{
	
}

}

#endif
//namespace JStl