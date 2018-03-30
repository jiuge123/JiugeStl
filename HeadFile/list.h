#ifndef  JIUGESTL_HEADFILE_LIST
#define  JIUGESTL_HEADFILE_LIST

#include <cassert>

#include <initializer_list>

#include "iterator.h"
#include "uninitialized.h"
#include "allocator.h"
#include "util.h"

//包含list

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
struct list_iterator :public JStl::iterator<JStl::bidrection_iterator_tag, T>
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
struct list_const_iterator :public JStl::iterator<JStl::bidrection_iterator_tag, T>
{
	typedef T                                 value_type;
	typedef const T*                          pointer;
	typedef const T&                          reference;
	typedef list_node<T>*					  node_ptr;
	typedef list_const_iterator<T>            self;

	node_ptr node_;

	list_const_iterator() = default;

	list_const_iterator(node_ptr x) :node_(x) {}

	list_const_iterator(const list_iterator& rhs) :node_(rhs.node_) {}

	list_const_iterator(const list_const_iterator& rhs) :node_(rhs.node_) {}

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

template<typename T, typename Alloc = JStl::allocator<T>>
class list{
public:
	typedef JStl::allocator<T>							data_allocator;
	typedef JStl::allocator<T>							allocator_type;
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
	
	typedef list_node<T>								list_node;

private:
	list_node node_;
	size_type size_;

public:
	//构造，拷贝构造，移动构造，析构，拷贝赋值，移动赋值
	list();
};

template<typename T>
void fill_init(size_type n, const value_type& value)
{

}

template<typename T>
list::list()
{

}
	
}//namespaec JStl;
#endif