#ifndef  JIUGESTL_HEADFILE_LIST
#define  JIUGESTL_HEADFILE_LIST

#include <cassert>

#include <initializer_list>

#include "iterator.h"
#include "uninitialized.h"
#include "allocator.h"
#include "util.h"

//包含list 双向环状链表

namespace JStl{

template <class T>
struct list_node 
{
	typedef list_node<T>* node_ptr;

	node_ptr prev;
	node_ptr next;
	T value;

	list_node() = default;

	list_node(const T &v) :value(v){}

	list_node(T &&v) :value(move(v)){}

	void unlink()
	{
		prev = next = self();
	}

	node_ptr self()
	{
		return static_cast<node_ptr>(&*this);
	}
};

//iterator

template <class T>
struct list_iterator :public iterator<bidrection_iterator_tag, T>
{
	typedef T                                 value_type;
	typedef T*                                pointer;
	typedef T&                                reference;
	typedef list_node<T>*					  node_ptr;
	typedef list_iterator<T>                  self;

	node_ptr node_; 

	list_iterator() = default;

	list_iterator(node_ptr x) :node_(x) {}

	list_iterator(const list_iterator& rhs) :node_(rhs.node_) {}

	reference operator*() const
	{
		return node_->value;
	}
	
	pointer operator->() const 
	{ 
		return &(operator*()); 
	}

	self& operator++()
	{
		assert(node_ != nullptr);
		node_ = node_->next;
		return *this;
	}

	self operator++(int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}

	self& operator--()
	{
		assert(node_ != nullptr);
		node_ = node_->prev;
		return *this;
	}

	self operator--(int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}

	bool operator==(const self& rhs) const 
	{ 
		return node_ == rhs.node_; 
	}
	
	bool operator!=(const self& rhs) const 
	{ 
		return node_ != rhs.node_; 
	}

};

template <class T>
struct list_const_iterator :public iterator<bidrection_iterator_tag, T>
{
	typedef T                                 value_type;
	typedef const T*                          pointer;
	typedef const T&                          reference;
	typedef list_node<T>*					  node_ptr;
	typedef list_const_iterator<T>            self;

	node_ptr node_;

	list_const_iterator() = default;

	list_const_iterator(node_ptr x) :node_(x) {}

	list_const_iterator(const list_iterator<T>& rhs) :node_(rhs.node_) {}

	list_const_iterator(const list_const_iterator<T>& rhs) :node_(rhs.node_) {}

	reference operator*() const
	{
		return node_->value;
	}

	pointer operator->() const
	{
		return &(operator*());
	}

	self& operator++()
	{
		assert(node_ != nullptr);
		node_ = node_->next;
		return *this;
	}

	self operator++(int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}

	self& operator--()
	{
		assert(node_ != nullptr);
		node_ = node_->prev;
		return *this;
	}

	self operator--(int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}

	bool operator==(const self& rhs) const
	{
		return node_ == rhs.node_;
	}

	bool operator!=(const self& rhs) const
	{
		return node_ != rhs.node_;
	}

//list

};

template<typename T, typename Alloc = allocator<T>>
class list{
public:
	typedef JStl::allocator<T>							data_allocator;
	typedef JStl::allocator<list_node<T>>				node_allocator;

	typedef typename allocator<T>::value_type			value_type;
	typedef typename allocator<T>::pointer				pointer;
	typedef typename allocator<T>::const_pointer		const_pointer;
	typedef typename allocator<T>::reference			reference;
	typedef typename allocator<T>::const_reference		const_reference; 
	typedef typename allocator<T>::size_type			size_type;
	typedef typename allocator<T>::difference_type		difference_type;

	typedef JStl::list_iterator<T>						iterator;
	typedef JStl::list_const_iterator<T>                const_iterator;
	typedef JStl::reverse_iterator<iterator>			reverse_iterator;
	typedef JStl::reverse_iterator<const_iterator>		const_reverse_iterator;
	
	typedef list_node<T>*								node_ptr;

private:
	node_ptr node_;   //指向尾部
	size_type size_;

	void fill_init(size_type n, const value_type& value);
	
	template<typename Iter>
	void copy_init(Iter first, Iter last);

	void fill_assign(size_type n, const value_type& value);

	template<typename Iter>
	void copy_assign(Iter first, Iter last);

	//插入尾部节点
	void link_node_at_back(node_ptr first, node_ptr last);
	
	//插入头部节点
	void link_node_at_first(node_ptr first, node_ptr last);

	template<class... Args>
	node_ptr create_node(Args&& ...args);

	void destory_node(node_ptr);
	
public:
	//构造，拷贝构造，移动构造，析构，拷贝赋值，移动赋值
	list();

	list(size_type n); 

	list(size_type n,const value_type& v);

	template<typename Iter, typename std::enable_if<
		JStl::is_input_iterator<Iter>::value, int>::type = 0>
	list(Iter first,Iter last);

	list(const std::initializer_list<T>& l);

	list(const list& rhs);

	list(list &&rhs);

	list& operator=(const list& rhs);

	list& operator=(list &&rhs);

	list& operator=(std::initializer_list<T> l);

	~list();
public:
	//迭代器相关操作
	iterator begin()
	{
		return node_->next;
	}

	const_iterator begin() const
	{
		return node_->next;
	}
	
	iterator end()
	{
		return node_;
	}
	
	const_iterator end() const
	{
		return node_;
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(end());
	}
	
	reverse_iterator rbegin() const
	{
		return reverse_iterator(end());
	}

	reverse_iterator rend()
	{
		return reverse_iterator(begin());
	}

	reverse_iterator rend() const
	{
		return reverse_iterator(begin());
	}
public:
	//普通函数
	void swap(list &rhs)
	{
		JStl::swap(node_, rhs.node_);
		JStl::swap(size_, rhs.size_);
	}

	size_t size()
	{
		return size_;
	}
public:
	//成员函数
	void assign(size_type n, const value_type& value);
	template <class Iter, typename std::enable_if<
		JStl::is_input_iterator<Iter>::value, int>::type = 0>
	void assign(Iter first, Iter last);
	void assign(std::initializer_list<value_type> il);

	void clear();

	iterator insert(const_iterator pos, size_type n, const value_type& value);
	template<typename Iter>
	iterator insert(const_iterator pos, Iter begin, Iter end);

	iterator erase(const_iterator begin, const_iterator end);
};

template<typename T, typename Alloc = JStl::allocator<T>>
template<class... Args>
typename list<T, Alloc>::node_ptr 
list<T, Alloc>::create_node(Args&& ...args)
{
	node_ptr p = node_allocator::allocate(1);
	try{
		data_allocator::construct(address_of(p->value),JStl::forward<Args>(args)...);
		p->prev = nullptr;
		p->next = nullptr;
	}
	catch (...)
	{
		node_allocator::deallocate(p);
		throw;
	}
	return p;
}

template<typename T,typename Alloc = allocator<T>>
void list<T, Alloc>::destory_node(node_ptr ptr)
{
	data_allocator::destroy(address_of(ptr->value));
	node_allocator::deallocate(ptr);
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::link_node_at_first(node_ptr first, node_ptr last)
{
	first->next = node_->next;
	last->prev = node_;
	node_->next->prev = first;
	node->next = last;
}

template<typename T,typename Alloc = allocator<T>>
void list<T, Alloc>::link_node_at_back(node_ptr first, node_ptr last)
{
	first->prev = node_->prev;
	last->next = node_;
	node_->prev->next = first;
	node_->prev = last;
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::fill_init(size_type n, const value_type& value)
{
	node_ = node_allocator::allocate(1);
	node_->unlink();
	size_ = n;
	try{
		for( ; n > 0; --n ){
			node_ptr node = create_node(value);
			link_node_at_back(node, node);
		}
	}
	catch (...){
		clear();
		node_allocator::deallocate(node_);
		node_ = nullptr;
		throw;
	}
}

template<typename T, typename Alloc = JStl::allocator<T>>
template<class Iter>
void list<T, Alloc>::copy_init(Iter first, Iter last)
{
	node_ = node_allocator::allocate(1);
	node_->unlink();
	size_ = JStl::distance(first, last);
	size_type n = size_;
	try{
		while (n--){
			node_ptr node = create_node(*first);
			link_node_at_back(node, node);
			++first;
		}
	}
	catch (...){
		clear();
		node_allocator::deallocate(node_);
		node_ = nullptr;
		throw;
	}	
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::fill_assign(size_type n, const value_type& value)
{
	auto i = begin();
	auto end = end();
	for (; n > 0 && i != end; --n, ++i){
		*i = value;
	}
	if (n > 0){
		insert(end, n, value); //迭代器先完成，还有未插入
	}
	else{
		erase(i, end);		//插入先完成，删除后面的原节点
	}
}

template<typename T, typename Alloc = JStl::allocator<T>>
template<class Iter>
void list<T, Alloc>::copy_assign(Iter first, Iter last)
{
	auto i = begin();
	auto end = end();
	for (; first != last && i != end; ++first, ++i){
		*i = *first;
	}
	if (first != last){
		insert(end, first, last);
	}
	else{
		erase(i, end);		
	}
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>::list()
{
	fill_init(0, value_type());
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>::list(size_type n)
{
	fill_init(n, value_type());
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>::list(size_type n,const value_type& v)
{
	fill_init(n, v);
}

template<typename T, typename Alloc = allocator<T>>
template<typename Iter, typename std::enable_if<
	JStl::is_input_iterator<Iter>::value, int>::type = 0>
list<T, Alloc>::list(Iter first, Iter last)
{
	copy_init(first, last);
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>::list(const std::initializer_list<T> &l)
{
	copy_init(l.begin(), l.end());
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>::list(const list& rhs)
{
	copy_init(rhs.begin(), rhs.end());
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>::list(list&& rhs) :node_(rhs.node_), size_(rhs.size_)
{
	rhs.node_ = nullptr;
	rhs.size_ = 0;
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>& list<T, Alloc>::operator=(const list& rhs)
{
	if (this != &rhs){
		assign(rhs.begin(), rhs.end());
	}
	return *this;
}


template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>& list<T, Alloc>::operator=(list&& rhs)
{
	if (this != &rhs){
		list tmp(JStl::move(rhs));
		swap(tmp);
	}
	return *this;
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>& list<T, Alloc>::operator=(std::initializer_list<T> l)
{
	list tmp(l.begin(),l.end());
	swap(tmp);
	return *this;
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>::~list()
{
	if (node_){
		clear();
		node_allocator::deallocate(node_);
		node_ = nullptr;
	}
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::assign(size_type n, const value_type& value)
{
	fill_assign(n, value);
}

template<typename T, typename Alloc = allocator<T>>
template <class Iter, typename std::enable_if<
	JStl::is_input_iterator<Iter>::value, int>::type = 0>
void list<T, Alloc>::assign(Iter first, Iter last)
{
	copy_assign(first, last);
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::assign(std::initializer_list<value_type> il)
{
	copy_assign(il.begin(), il.end());
}

//template<typename T, typename Alloc = allocator<T>>
//typename list<T, Alloc>::iterator  
//list<T, Alloc>::insert(const_iterator pos, size_type n, const value_type& value)
//{
//
//}
//
//template<typename T, typename Alloc = allocator<T>>
//template<typename Iter>
//typename list<T, Alloc>::iterator
//iterator insert(const_iterator pos, Iter begin, Iter end)
//{
//
//}
//
//template<typename T, typename Alloc = allocator<T>>
//typename list<T, Alloc>::iterator
//list<T, Alloc>::erase(const_iterator begin, const_iterator end)
//{
//
//}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::clear()
{
	if (size_ != 0){
		node_ptr cur = node_->next;
		for (auto next = cur->next; cur != node_; cur = next, next = cur->next){
			destory_node(cur->self());
		}
		node_->unlink();
		size_ = 0;
	}
}

};//namespaec JStl;
#endif