#ifndef  JIUGESTL_HEADFILE_FUNCTIONAL
#define  JIUGESTL_HEADFILE_DEQUE

#include<cassert>

#include <initializer_list>

#include "iterator.h"
#include "uninitialized.h"
#include "allocator.h"

//包含双端队列

namespace JStl{

template<typename T>
struct _deque_buf_size
{
	static const size_t value = sizeof(T) < 256 ? 1024 / sizeof(T) : 8;
};


//iterator
template<typename T,typename Ref,typename Ptr>
struct deque_iterator:public random_access_iterator_tag
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

	static const size_type buffer_size = _deque_buf_size<T>::value;

	value_pointer cur;      //指向缓冲区现行元素
	value_pointer first;	//指向缓冲区头
	value_pointer last;		//指向缓冲区尾	
	map_pointer   node;		//指向管控中心
	//管控中心是一个T**,存放着指向各个队列头部的指针

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
		if (*this != rhs)
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

	// 转到另一个缓冲区
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
		else // 要跳到其他的缓冲区
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

template <class T, class Alloc = allocator<T>>
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
	typedef JStl::reverse_iterator<iterator>         reverse_iterator;
	typedef JStl::reverse_iterator<const_iterator>   const_reverse_iterator;

	static const size_type buffer_size = _deque_buf_size<T>::value;

private:
	iterator       begin_;     // 指向第一个节点
	iterator       end_;       // 指向最后一个结点
	map_pointer    map_;       // 指向管控中心
	size_type      map_size_;  // map 内指针的数目

	//为map分配内存
	map_pointer create_map(size_type n);

	//构造缓冲区
	void create_buffer(map_pointer nstart, map_pointer nfinish);

	//构造map
	void map_init(size_type n);

	void fill_init(size_type n, const value_type& value);

	template <class Iter>
	void copy_init(Iter first, Iter last, input_iterator_tag);
	template <class Iter>
	void copy_init(Iter first, Iter last, forward_iterator_tag);

	void require_capacity(size_type n, bool isfront);

public:
	//构造，拷贝构造，移动构造，析构，拷贝赋值，移动赋值
	deque();

	deque(size_type n);

	deque(size_type n, const value_type& value);

	template<typename Iter, typename std::enable_if<
		is_input_iterator<Iter>::value, int>::type = 0>
	deque(Iter first, Iter last);

public:
	//成员函数
	template <class ...Args>
	void emplace_back(Args&& ...args);
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
void deque<T, Alloc>::create_buffer(map_pointer nstart, map_pointer nfinish)
{
	map_pointer cur;
	try{
		for (cur = nstart; cur <= nfinish; ++cur){
			*cur = data_allocator::allocate(buffer_size);
		}
	}
	catch (...){
		while (cur != nstart){
			--cur;
			data_allocator::deallocate(*cur, buffer_size);
			*cur = nullptr;
		}
		throw;
	}
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::map_init(size_type n)
{
	const size_type nNode = n / buffer_size + 1;
	map_size_ = JStl::max((size_type)8, nNode + 2);	// 需要分配的缓冲区个数 
	try{
		map_ = create_map(map_size_);
	}
	catch (...){
		map_allocator::deallocate(map_, map_size_);
		map_ = nullptr;
		map_size_ = 0;
		throw;
	}
	// 指向数据的开始和结束 而且指向中间
	map_pointer nstart = map_ + (map_size_ - n) / 2;
	map_pointer nfinish = nstart + n - 1;
	try{
		create_buffer(nstart, nfinish);
	}
	catch (...){
		map_allocator::deallocate(map_, map_size_);
		map_ = nullptr;
		map_size_ = 0;
		throw;
	}
	begin_.set_node(nstart);
	end_.set_node(nfinish);
	begin_.cur = begin_.first;
	end_.cur = end_.first + (n % buffer_size);
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::fill_init(size_type n, const value_type& value)
{
	map_init(n);
	if (n != 0)
	{
		for (auto cur = begin_.node; cur < end_.node; ++cur)
		{
			JStl::uninitialized_fill(*cur, *cur + buffer_size, value);
		}
		JStl::uninitialized_fill(end_.first, end_.cur, value);
	}
}

template<typename T, typename Alloc = allocator<T>>
template<typename Iter>
void deque<T, Alloc>::copy_init(Iter first, Iter last, input_iterator_tag)
{
	const size_type n = JStl::distance(first, last);
	map_init(n);
	for (; first != last; ++first)
		emplace_back(*first);
}

template<typename T, typename Alloc = allocator<T>>
template<typename Iter>
void deque<T, Alloc>::copy_init(Iter first, Iter last, forward_iterator_tag)
{
	const size_type n = JStl::distance(first, last);
	map_init(n);
	for (auto cur = begin_.node; cur < end_.node; ++cur)
	{
		auto next = first;
		JStl::advance(next, buffer_size);			//每次拷贝buffer_size个数
		JStl::uninitialized_copy(first, next, *cur);//到cur中
		first = next;
	}
	JStl::uninitialized_copy(first, last, end_.first);
}

//template<typename T, typename Alloc = allocator<T>>
//void deque<T, Alloc>::require_capacity(size_type n, bool isfront)
//{
//	if (isfront && (static_cast<size_type>(begin_.cur - begin_.first) < n))
//	{
//		const size_type need_buffer = (n - (begin_.cur - begin_.first)) / buffer_size + 1;
//		if (need_buffer > static_cast<size_type>(begin_.node - map_))
//		{
//			reallocate_map_at_front(need_buffer);
//			return;
//		}
//		create_buffer(begin_.node - need_buffer, begin_.node - 1);
//	}
//	else if (!isfront && (static_cast<size_type>(end_.last - end_.cur - 1) < n))
//	{
//		const size_type need_buffer = (n - (end_.last - end_.cur - 1)) / buffer_size + 1;
//		if (need_buffer > static_cast<size_type>((map_ + map_size_) - end_.node - 1))
//		{
//			reallocate_map_at_back(need_buffer);
//			return;
//		}
//		create_buffer(end_.node + 1, end_.node + need_buffer);
//	}
//} 

template<typename T,typename Alloc = allocator<T>>
deque<T,Alloc>::deque()
{
	fill_init(0, value_type());
}

template<typename T, typename Alloc = allocator<T>>
deque<T, Alloc>::deque(size_type n)
{
	fill_init(n, value_type());
}

template<typename T, typename Alloc = allocator<T>>
deque<T, Alloc>::deque(size_type n,const value_type& value)
{
	fill_init(n, value);
}

template<typename T, typename Alloc = allocator<T>>
template <class Iter, typename std::enable_if<
	JStl::is_input_iterator<Iter>::value, int>::type = 0>
deque<T, Alloc>::deque(Iter first, Iter last)
{
	copy_init(first, last, iterator_category(first));
}

template<typename T, typename Alloc = allocator<T>>
template <class ...Args>
void deque<T, Alloc>::emplace_back(Args&& ...args)
{
	//由于每个buffer只能存放size-1个数据
	//所以cur最多到last-1
	if (end_.cur != end_.last - 1){
		data_allocator::construct(end_.cur, JStl::forward<Args&&>(args)...);
	}
	else{
		//require_capacity(1, false);
		data_allocator::construct(end_.cur, JStl::forward<Args>(args)...);
	}
	++end_;
}

}//na`espace JStl
#endif
