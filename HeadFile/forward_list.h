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
template <class T>
struct flist_node 
{
	typedef flist_node<T>* node_ptr;

	node_ptr next;
	T value;

	flist_node() = default;

	flist_node(const T &v) :value(v){}

	flist_node(T &&v) :value(move(v)){}

	node_ptr self()
	{
		return static_cast<node_ptr>(&*this);
	}
};

//iterator

template <class T>
struct flist_iterator :public forward_iterator_tag
{
	typedef forward_iterator_tag			  iterator_category;
	typedef T                                 value_type;
	typedef T*                                pointer;
	typedef T&                                reference;
	typedef ptrdiff_t						  difference_type;

	typedef flist_node<T>*					  node_ptr;
	typedef flist_iterator<T>                 self;

	node_ptr node_;

	flist_iterator() = default;

	flist_iterator(node_ptr x) :node_(x) {}

	flist_iterator(const flist_iterator& rhs) :node_(rhs.node_) {}

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
struct flist_const_iterator :public forward_iterator_tag
{
	typedef forward_iterator_tag			  iterator_category;
	typedef T                                 value_type;
	typedef const T*                          pointer;
	typedef const T&                          reference;
	typedef ptrdiff_t						  difference_type;

	typedef flist_node<T>*					  node_ptr;
	typedef flist_const_iterator<T>           self;

	node_ptr node_;

	flist_const_iterator() = default;

	flist_const_iterator(node_ptr x) :node_(x) {}

	flist_const_iterator(const list_iterator<T>& rhs) :node_(rhs.node_) {}

	flist_const_iterator(const flist_const_iterator<T>& rhs) :node_(rhs.node_) {}

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

	bool operator==(const self& rhs) const
	{
		return node_ == rhs.node_;
	}

	bool operator!=(const self& rhs) const
	{
		return node_ != rhs.node_;
	}
};

}

#endif
//namespace JStl