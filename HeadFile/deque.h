#ifndef  JIUGESTL_HEADFILE_FUNCTIONAL
#define  JIUGESTL_HEADFILE_DEQUE

#include<cassert>

#include <initializer_list>

#include "iterator.h"
#include "uninitialized.h"
#include "allocator.h"

//����˫�˶���

namespace JStl{

template<typename T>
size_t _deque_buf_size()
{
	return sizeof(T) < 256 ? 2048 / sizeof(T) : 8;
}

//iterator
template<typename T,typename Ref,typename Ptr>
struct deque_iterator
{
	typedef deque_iterator<T, T&, T*>				iterator;
	typedef deque_iterator<T, const T&, const T*>	const_iterator;
	typedef deque_iterator							self;
	typedef random_access_iterator_tag				iterator_category;

	typedef T            value_type;
	typedef Ptr          pointer;
	typedef Ref          reference;
	typedef size_t       size_type;
	typedef ptrdiff_t    difference_type;
	typedef T*           value_pointer;
	typedef T**          map_pointer;

	const size_type buffer_size = _deque_buf_size<T>();

	value_pointer cur;      //ָ�򻺳�������Ԫ��
	value_pointer first;	//ָ�򻺳���ͷ
	value_pointer last;		//ָ�򻺳���β	
	map_pointer   node;		//ָ��ܿ�����
	//�ܿ�������һ��int[]*,�����ָ���������ͷ����ָ��

	deque_iterator() 
		:cur(nullptr), first(nullptr), last(nullptr), node(nullptr)
	{}

	deque_iterator(value_pointer v, map_pointer n)
		:cur(v), first(*n), last(*n + buffer_size), node(n) 
	{}

	deque_iterator(const iterator& rhs)
		:cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node)
	{}

	deque_iterator(iterator&& rhs) 
		: cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node)
	{
		rhs.cur = nullptr;
		rhs.first = nullptr;
		rhs.last = nullptr;
		rhs.node = nullptr;
	}

	deque_iterator(const const_iterator& rhs)
		:cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node)
	{}

	self& operator=(const iterator& rhs)
	{
		if (this != &rhs)
		{
			cur = rhs.cur;
			first = rhs.first;
			last = rhs.last;
			node = rhs.node;
		}
		return *this;
	}

	self& operator=(iterator&& rhs)
	{
		if (this != &rhs)
		{
			cur = rhs.cur;
			first = rhs.first;
			last = rhs.last;
			node = rhs.node;
		}
		rhs.cur = nullptr;
		rhs.first = nullptr;
		rhs.last = nullptr;
		rhs.node = nullptr;
		return *this;
	}

	// ת����һ��������
	void set_node(map_pointer new_node)
	{
		node = new_node;
		first = *new_node;
		last = first + buffer_size;
	}

	reference operator*()  const 
	{ 
		return *cur; 
	}

	pointer operator->() const 
	{ 
		return cur; 
	}

	difference_type operator-(const self& x) const
	{
		return static_cast<difference_type>(buffer_size * (node - x.node)
			+ (cur - first) - (x.cur - x.first));
	}

};

template <class T, class Alloc = allocator>
class deque{

};

}
#endif
