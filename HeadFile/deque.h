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

	static const size_type buffer_size;

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

};

template<typename T, typename Ref, typename Ptr>
size_t deque_iterator<T, Ref, Ptr>::buffer_size = _deque_buf_size<T>();

template <class T, class Alloc = allocator>
class deque{

};

}
#endif
