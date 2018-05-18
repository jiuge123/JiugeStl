#ifndef  JIUGESTL_HEADFILE_FORWARDLIST
#define  JIUGESTL_HEADFILE_FORWARDLIST

#include <cassert>
#include<iostream>
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
	_flist_node(T value) :data(value)
	{}
	T data;
};

_flist_node_base* _flist_make_link(_flist_node_base *prev, _flist_node_base *newnode)
{
	newnode->next = prev->next;
	prev->next = newnode;
	return newnode;
}

size_t _flist_size(_flist_node_base *node)
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

	flist_iterator() :_flist_iterator_base(nullptr)
	{}

	flist_iterator(flist_node *x) :_flist_iterator_base(x)
	{}

	flist_iterator(const iterator &x) :_flist_iterator_base(x.node_)
	{}

	reference operator*() const
	{
		assert(node_ != nullptr);
		return ((flist_node*)node_)->data;
	}

	pointer operator->() const
	{
		return &(operator*());
	}

	self& operator++()
	{
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

	void fill_init(size_type n, const value_type& value);  
	template<typename Iter>
	void copy_init(Iter first, Iter last);

	iterator fill_insert(const_iterator pos, size_type n, const value_type& value);
	template<typename Iter>
	iterator copy_insert(const_iterator pos, Iter first, Iter last);
	
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
		return iterator(static_cast<node_ptr>(head_.next));
	}

	const_iterator begin() const
	{
		return iterator(static_cast<node_ptr>(head_.next));
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

	iterator before_begin()
	{
		return iterator(static_cast<node_ptr>(&head_));
	}

	const_iterator cbefore_begin() const
	{
		return iterator(static_cast<node_ptr>(&head_));
	}

	data_allocator get_allocator()
	{
		return data_allocator();
	}

public:
	reference front() 
	{
		return (*begin());
	}

	const_reference front() const
	{
		return (*begin());
	}

	void swap(forward_list& rhs)
	{
		JStl::swap(head_, rhs.head_);
	}

	bool empty()
	{
		return head_.next == nullptr;
	}

	size_type size()
	{
		return _flist_size(&head_);
	}

public:
	void assign(size_type n, const value_type& value);
	template <class Iter, typename std::enable_if<
		JStl::is_input_iterator<Iter>::value, int>::type = 0>
		void assign(Iter first, Iter last);
	void assign(std::initializer_list<value_type> il);

	//在pos后面构造
	template<class ...Args>
	iterator emplace_after(iterator pos, Args&& ...args);
	
	//在最开始构造，即head后面
	template<class ...Args>
	void emplace_front(Args&& ...args);

	iterator insert_after(const_iterator pos, const value_type& value);
	iterator insert_after(const_iterator pos, value_type&& value);
	iterator insert_after(const_iterator pos, size_type n, const value_type& value);
	template<typename Iter, typename std::enable_if<
		JStl::is_input_iterator<Iter>::value, int>::type = 0 >
	iterator insert_after(const_iterator pos, Iter begin, Iter end);
	iterator insert_after(const_iterator pos, std::initializer_list<value_type> l);

	//在pos后面删除 返回删除之后的iter
	iterator erase_after(iterator pos);

	void clear();
};

template<typename T, typename Alloc = allocator<T>>
template<class... Args>
typename forward_list<T, Alloc>::node_ptr
forward_list<T, Alloc>::create_node(Args&& ...args)
{
	node_ptr node = node_allocator::allocate();
	try{
		node_allocator::construct(node, JStl::forward<Args>(args)...);
		node->next = nullptr;
		return node;
	}
	catch (...){
		node_allocator::deallocate(node);
		throw;
	}
}

template<typename T, typename Alloc = allocator<T>>
void forward_list<T, Alloc>::destroy_node(node_ptr node)
{
	node_allocator::destroy(node);
	node_allocator::deallocate(node);
}

template<typename T, typename Alloc = allocator<T>>
void forward_list<T, Alloc>::fill_init(size_type n, const value_type& value)
{
	head_.next = nullptr;
	auto p = &head_;
	try{
		for (; n > 0; --n){
			auto node = create_node(value);
			p = _flist_make_link(p, node);
		}
	}
	catch (...){
		clear();
		throw;
	}
}

template<typename T, typename Alloc = allocator<T>>
template<typename Iter>
void forward_list<T, Alloc>::copy_init(Iter first, Iter last)
{
	head_.next = nullptr;
	auto p = &head_;
	size_type n = JStl::distance(first, last);
	try{
		for (; n > 0; --n){
			auto node = create_node(*first);
			p = _flist_make_link(p, node);
			++first;
		}
	}
	catch (...){
		clear();
		throw;
	}
}

template<typename T, typename Alloc = allocator<T>>
typename forward_list<T, Alloc>::iterator 
forward_list<T, Alloc>::fill_insert(const_iterator pos, size_type n, const value_type& value)
{
	auto p = pos.node_;
	size_type n2 = n;
	try{
		for (; n > 0; --n){
			auto node = create_node(value);
			p = _flist_make_link(p, node);
		}
	}
	catch (...){
		for (n = n2 - n; n > 0; --n){
			erase_after(pos);
		}
		throw;
	}
	return iterator((++pos).node_);
}

template<typename T, typename Alloc = allocator<T>>
template<typename Iter>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::copy_insert(const_iterator pos, Iter first, Iter last)
{
	auto p = pos.node_;
	size_type n = JStl::distance(first, last), n2 = n;
	try{
		for (; n > 0; --n){
			auto node = create_node(*first);
			p = _flist_make_link(p, node);
			++first;
		}
	}
	catch (...){
		for (n = n2 - n; n > 0; --n){
			erase_after(pos);
		}
		throw;
	}
	return iterator((++pos).node_);
}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>::forward_list()
{
	fill_init(0, value_type);
}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>::forward_list(size_type n)
{
	fill_init(n, value_type());
}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>::forward_list(size_type n, const value_type& v)
{
	fill_init(n, v);
}

template<typename T, typename Alloc = allocator<T>>
template<typename Iter, typename std::enable_if<
	JStl::is_input_iterator<Iter>::value, int>::type = 0>
forward_list<T, Alloc>::forward_list(Iter first, Iter last)
{
	copy_init(first, last);
}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>::forward_list(std::initializer_list<T> l)
{
	copy_init(l.begin(), l.end());
}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>::forward_list(const forward_list& rhs)
{
	copy_init(rhs.begin(), rhs.end());
}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>::forward_list(forward_list &&rhs)
{
	head_ = rhs.head_;
	rhs.head_.next = nullptr;
}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>& forward_list<T, Alloc>::operator=(const forward_list& rhs)
{
	clear();
	copy_init(rhs.begin(),rhs.end());
	return *this;
}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>& forward_list<T, Alloc>::operator=(forward_list &&rhs)
{
	swap(rhs);
	rhs.head_.next = nullptr;
	return *this;
}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>& forward_list<T, Alloc>::operator=(std::initializer_list<T> l)
{
	clear();
	copy_init(l.begin(), l.end());
	return *this;
}

template<typename T, typename Alloc = allocator<T>>
forward_list<T, Alloc>::~forward_list()
{
	clear();
}

template<typename T, typename Alloc = allocator<T>>
void forward_list<T, Alloc>::assign(size_type n, const value_type& value)
{
	clear();
	fill_init(n, value);
}

template<typename T, typename Alloc = allocator<T>>
template <class Iter, typename std::enable_if<
	JStl::is_input_iterator<Iter>::value, int>::type = 0>
void forward_list<T, Alloc>::assign(Iter first, Iter last)
{
	clear();
	copy_init(first,last);
}

template<typename T, typename Alloc = allocator<T>>
void forward_list<T, Alloc>::assign(std::initializer_list<value_type> l)
{
	clear();
	copy_init(l.begin(), l.end());
}

template<typename T, typename Alloc = allocator<T>>
template<class ...Args>
typename forward_list<T, Alloc>::iterator 
forward_list<T, Alloc>::emplace_after(iterator pos, Args&& ...args)
{
	node_ptr new_node = create_node(JStl::forward<Args>(args)...);
	_flist_make_link(pos.node_, new_node);
	return pos;
}

template<typename T, typename Alloc = allocator<T>>
template<class ...Args>
void forward_list<T, Alloc>::emplace_front(Args&& ...args)
{
	emplace_after(before_begin(), JStl::forward<Args>(args)...);
}

template<typename T, typename Alloc = allocator<T>>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::insert_after(const_iterator pos, const value_type& value)
{
	return fill_insert(pos, 1, value);
}

template<typename T, typename Alloc = allocator<T>>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::insert_after(const_iterator pos, value_type&& value)
{
	auto node = create_node(JStl::move(value));
	_flist_make_link(pos.node_, node);
	return iterator(static_cast<node_ptr>((++pos).node_));
}

template<typename T, typename Alloc = allocator<T>>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::insert_after(const_iterator pos, size_type n, const value_type& value)
{
	return fill_insert(pos, n, value);
}

template<typename T, typename Alloc = allocator<T>>
template<typename Iter, typename std::enable_if<
	JStl::is_input_iterator<Iter>::value, int>::type = 0 >
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::insert_after(const_iterator pos, Iter begin, Iter end)
{
	return copy_insert(pos, begin, end);
}

template<typename T, typename Alloc = allocator<T>>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::insert_after(const_iterator pos, std::initializer_list<value_type> l)
{
	return copy_insert(pos, l.begin(), l.end());
}

template<typename T, typename Alloc = allocator<T>>
typename forward_list<T, Alloc>::iterator
forward_list<T, Alloc>::erase_after(iterator pos)
{
	if (pos.node_->next != nullptr){
		auto p = pos.node_->next;
		pos.node_->next = p->next;
		destroy_node(static_cast<node_ptr>(p));
	}
	return iterator(++pos);
}

template<typename T, typename Alloc = allocator<T>>
void forward_list<T, Alloc>::clear()
{
	auto p = head_.next;
	while (p != nullptr){
		auto pnext = p->next;
		destroy_node(static_cast<node_ptr>(p));
		p = pnext;
	}
	head_.next = nullptr;
}

}

#endif
//namespace JStl