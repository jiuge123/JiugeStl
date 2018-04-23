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
	return sizeof(T) < 256 ? 1024 / sizeof(T) : 8;
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
	//�ܿ�������һ��T**,�����ָ���������ͷ����ָ��

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

	self& operator++()
	{
		++cur;
		if (cur == last)
		{ 
			set_node(node + 1);
			cur = first;
		}
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
		if (cur == first)
		{ 
			set_node(node - 1);
			cur = last;
		}
		--cur;
		return *this;
	}

	self operator--(int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}

	self& operator+=(difference_type n)
	{
		const auto offset = n + (cur - first);
		if (offset >= 0 && offset < static_cast<difference_type>(buffer_size))
		{ 
			cur += n;
		}
		else // Ҫ���������Ļ�����
		{
			auto m1 = offset / buffer_size;
			auto m2 = offset % buffer_size;
			set_node(node + m1);
			cur = first;
			cur += m2;
		}
		return *this;
	}

	self& operator-=(difference_type n)
	{
		return *this += -n;
	}

	self operator+(difference_type n) const
	{
		self tmp = *this;
		return tmp += n;
	}

	self operator-(difference_type n) const
	{
		self tmp = *this;
		return tmp -= n;
	}

	reference operator[](difference_type n) const 
	{ 
		return *(*this + n); 
	}

	bool operator==(const self& rhs) const 
	{ 
		return cur == rhs.cur; 
	}
	
	bool operator< (const self& rhs) const
	{
		return node == rhs.node ? (cur < rhs.cur) : (node < rhs.node);
	}

	bool operator!=(const self& rhs) const 
	{ 
		return !(*this == rhs); 
	}

	bool operator>(const self& rhs) const 
	{ 
		return rhs < *this;
	}

	bool operator<=(const self& rhs) const 
	{ 
		return !(rhs < *this); 
	}

	bool operator>=(const self& rhs) const 
	{ 
		return !(*this < rhs); 
	}

};

template <class T, class Alloc = allocator>
class deque{
	typedef JStl::allocator<T>                       data_allocator;
	typedef JStl::allocator<T*>                      map_allocator;

	typedef typename data_allocator::value_type      value_type;
	typedef typename data_allocator::pointer         pointer;
	typedef typename data_allocator::const_pointer   const_pointer;
	typedef typename data_allocator::reference       reference;
	typedef typename data_allocator::const_reference const_reference;
	typedef typename data_allocator::size_type       size_type;
	typedef typename data_allocator::difference_type difference_type;
	typedef pointer*                                 map_pointer;
	typedef const_pointer*                           const_map_pointer;

	typedef deque_iterator<T, T&, T*>                iterator;
	typedef deque_iterator<T, const T&, const T*>    const_iterator;
	typedef JStl::reverse_iterator<iterator>        reverse_iterator;
	typedef JStl::reverse_iterator<const_iterator>  const_reverse_iterator;

	const size_type buffer_size = deque_buf_size<T>::value;

private:
	iterator       begin_;     // ָ���һ���ڵ�
	iterator       end_;       // ָ�����һ�����
	map_pointer    map_;       // ָ��ܿ�����
	size_type      map_size_;  // map ��ָ�����Ŀ

	//�����ڴ�
	map_pointer create_map(size_type n);

	//
	void map_init(size_type n);

	void fill_init(size_type n, const value_type& value);
public:
	//���죬�������죬�ƶ����죬������������ֵ���ƶ���ֵ
	deque();
};

template<typename T, typename Alloc = allocator<T>>
typename deque<T, Alloc>::map_pointer
deque<T, Alloc>::create_map(size_type size)
{
	map_pointer mp = nullptr;
	mp = map_allocator::allocate(size);
	for (size_type i = 0; i < size; ++i)
		*(mp + i) = nullptr;
	return mp;
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::map_init(size_type n)
{
	const size_type nNode = n / buffer_size + 1;
	map_size_ = nNode + 2;		// ��Ҫ����Ļ��������� + 2
	try{
		map_ = create_map(map_size_);
	}
	catch (...){
		map_ = nullptr;
		map_size_ = 0;
		throw;
	}

//	// �� nstart �� nfinish ��ָ�� map_ �����������
//	map_pointer nstart = map_ + (map_size_ - nNode) / 2;
//	map_pointer nfinish = nstart + nNode - 1;
//	try
//	{
//		create_buffer(nstart, nfinish);
//	}
//	catch (...)
//	{
//		map_allocator::deallocate(map_, map_size_);
//		map_ = nullptr;
//		map_size_ = 0;
//		throw;
//	}
//	begin_.set_node(nstart);
//	end_.set_node(nfinish);
//	begin_.cur = begin_.first;
//	end_.cur = end_.first + (nElem % buffer_size);
//}
//
//template<typename T, typename Alloc = allocator<T>>
//void deque<T, Alloc>::fill_init(size_type n, const value_type& value)
//{
//	map_init(n);
//	if (n != 0)
//	{
//		for (auto cur = begin_.node; cur < end_.node; ++cur)
//		{
//			mystl::uninitialized_fill(*cur, *cur + buffer_size, value);
//		}
//		mystl::uninitialized_fill(end_.first, end_.cur, value);
//	}
//}
//
//template<typename T,typename Alloc = allocator<T>>
//deque<T,Alloc>::deque()
//{
//	fill_init(0, value_type());
//}

}
#endif
