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

	_flist_node_base *node;

	_flist_iterator_base(_flist_node_base *x) :node(x)
	{}
	
	void incr()
	{
		node = node->next;
	}

	bool operator==(const _flist_iterator_base& x) const{
		return node == x.node;
	}

	bool operator!=(const _flist_iterator_base& x) const{
		return !(node == x.node);
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
	typedef _flist_node<T>  list_node;

	flist_iterator() :_flist_iterator_base(nullptr)
	{}

	flist_iterator(list_node *x) :_flist_iterator_base(x)
	{}

	flist_iterator(const list_iterator *x) :_flist_iterator_base(x.node)
	{}

	reference operator*() const
	{
		return (static_cast<list_node*>(node))->data;
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
	typedef JStl::flist_iterator<T,const T&,const T*>         
														const_iterator;
	typedef JStl::reverse_iterator<iterator>			reverse_iterator;
	typedef JStl::reverse_iterator<const_iterator>		const_reverse_iterator;

	typedef	_flist_node_base			flist_node_base;
	typedef	_flist_node<T>				flist_node;
	typedef	_flist_iterator_base		iterator_base;
};
}

#endif
//namespace JStl