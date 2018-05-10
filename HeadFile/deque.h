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
		if (offset >= 0 && offset < static_cast<difference_type>(buffer_size)){ 
			cur += n;
		}
		else{ // 要跳到其他的缓冲区
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

	void fill_assign(size_type n, const value_type& value);
	template <class Iter>
	void copy_assign(Iter first, Iter last, input_iterator_tag);
	template <class Iter>
	void copy_assign(Iter first, Iter last, forward_iterator_tag);

	//在头部更换map_
	void reallocate_map_at_front(size_type need_buffer);
	void reallocate_map_at_back(size_type need_buffer);

	//计算缓冲区越界后是否需要构造新的缓冲区和换map
	void require_capacity(size_type n, bool isfront);

	template <class... Args>
	iterator insert_aux(iterator pos, Args&& ...args);

public:
	//构造，拷贝构造，移动构造，析构，拷贝赋值，移动赋值
	deque();

	deque(size_type n);

	deque(size_type n, const value_type& value);

	template<typename Iter, typename std::enable_if<
		is_input_iterator<Iter>::value, int>::type = 0>
	deque(Iter first, Iter last);

	deque(std::initializer_list<T> l);

	deque(const deque& rhs);

	deque(deque &&rhs);

	deque& operator=(const deque& rhs);

	deque& operator=(deque&& rhs);

	deque& operator=(std::initializer_list<value_type> l);

	~deque();

public:
	//迭代器相关操作
	iterator begin()
	{
		return begin_;
	}

	const_iterator begin() const
	{
		return begin_;
	}

	iterator end()
	{
		return end_;
	}

	const_iterator end() const
	{
		return end_;
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

	const_iterator cbegin() const
	{
		return begin();
	}

	const_iterator cend() const
	{
		return end();
	}

	reverse_iterator rcbegin() const
	{
		return rbegin();
	}

	reverse_iterator rcend() const
	{
		return rend();
	}
public:
	//简单函数
	
	size_type size() const
	{
		return end_ - begin_;
	}

	void swap(deque& rhs)
	{
		if (this != &rhs){
			JStl::swap(begin_, rhs.begin_);
			JStl::swap(end_, rhs.end_);
			JStl::swap(map_, rhs.map_);
			JStl::swap(map_size_, rhs.map_size_);
		}
	}

	bool empty() const
	{
		return begin_ == end_;
	}

	reference operator[](size_type n)
	{
		if(n < size());
			return begin_[n];
		throw std::out_of_range("deque_index_out_of_range");
	}

	const_reference operator[](size_type n) const 
	{
		if(n < size());
			return begin_[n];
		throw std::out_of_range("deque_index_out_of_range");
	}

	reference at(size_type n) 
	{
		return (*this)[n];
	}

	const_reference at(size_type n) const 
	{
		return (*this)[n];
	}

	reference front()
	{
		assert(!empty());
		return begin_[0];
	}

	const_reference front() const 
	{
		assert(!empty());
		return *begin_;
	}

	reference back()
	{
		assert(!empty());
		return *(end_ - 1);
	}

	const_reference back() const
	{
		assert(!empty());
		return *(end_ - 1);
	}

public:
	//复杂函数
	// 减小容器容量
	void shrink_to_fit();

	void assign(size_type n, const value_type& value);
	template <class Iter, typename std::enable_if<
		JStl::is_input_iterator<Iter>::value, int>::type = 0>
	void assign(Iter first, Iter last);

	template <class ...Args>
	void emplace_front(Args&& ...args);
	template <class ...Args>
	void emplace_back(Args&& ...args);
	template <class ...Args>
	iterator emplace(iterator pos, Args&& ...args);

	void push_front(const value_type& value);
	void push_front(value_type&& value);

	void push_back(const value_type& value);
	void push_back(value_type&& value);

	void pop_front();
	void pop_back();

	iterator insert(iterator pos, const value_type& value);
	iterator insert(iterator pos, value_type&& value);
	void insert(iterator pos, size_type n, const value_type& value);
	template <class Iter, typename std::enable_if<
		JStl::is_input_iterator<Iter>::value, int>::type = 0>
	void insert(iterator pos, Iter first, Iter last);
	void insert(iterator pos, std::initializer_list<value_type> l);

	iterator erase(iterator pos);
	iterator erase(iterator first, iterator last);

	void resize(size_type new_size);
	void resize(size_type new_size, const value_type& value);
	void clear();
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
	map_size_ = JStl::max(static_cast<size_type>(8), nNode + 2);	// 需要分配的缓冲区个数 
	try{
		map_ = create_map(map_size_);
	}
	catch (...){
		map_allocator::deallocate(map_, map_size_);
		map_ = nullptr;
		map_size_ = 0;
		throw;
	}
	// 指向数据的开始和结束 而且指向map中间
	map_pointer nstart = map_ + (map_size_ - nNode) / 2;
	map_pointer nfinish = nstart + nNode - 1;
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

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::fill_assign(size_type n, const value_type& value)
{
	if (n > size()){
		JStl::fill_n(begin_, size(), value);
		insert(end_, n - size(), value);
	}
	else{
		JStl::fill_n(begin_, n, value);
		erase(begin_ + n, end_);
	}
}

template<typename T, typename Alloc = allocator<T>>
template <class Iter>
void deque<T, Alloc>::copy_assign(Iter first, Iter last, input_iterator_tag)
{
	auto begin = begin_;
	auto end = end_;
	for (; begin != end && first1 != last1; ++begin, ++first1){
		*begin = *first;
	}
	if (begin != end){
		//插入数目小于原有数目
		erase(first1, last1);
	}
	else{
		insert(end_, first, last);
	}
}

template<typename T, typename Alloc = allocator<T>>
template <class Iter>
void deque<T, Alloc>::copy_assign(Iter first, Iter last, forward_iterator_tag)
{
	const size_type len1 = size();
	const size_type len2 = JStl::distance(first, last);
	if (len1 < len2){
		auto next = first;
		JStl::advance(next, len1);
		JStl::copy(first, next, begin_);
		insert(end_, next, last);
	}
	else{
		erase(JStl::copy(first, last, begin_), end_);
	}
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::reallocate_map_at_front(size_type need_buffer)
{
	const size_type new_map_size = JStl::max(map_size_ * 2 ,
		map_size_ + need_buffer + 8);
	map_pointer new_map = create_map(new_map_size);
	const size_type old_buffer = end_.node - begin_.node + 1;
	//所需构造的缓冲区数目
	const size_type new_buffer = old_buffer + need_buffer;

	auto begin = new_map + (new_map_size - new_buffer) / 2;
	//begin 到 mid 是新的
	auto mid = begin + need_buffer;
	auto end = mid + old_buffer;
	create_buffer(begin, mid - 1);
	//将新的 map 中的指针指向原来的 buffer
	for (auto begin1 = mid, begin2 = begin_.node; begin1 != end; ++begin1, ++begin2)
		*begin1 = *begin2;

	// 更新数据
	map_allocator::deallocate(map_, map_size_);
	map_ = new_map;
	map_size_ = new_map_size;
	begin_ = iterator(*mid + (begin_.cur - begin_.first), mid);
	end_ = iterator(*(end - 1) + (end_.cur - end_.first), end - 1);
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::reallocate_map_at_back(size_type need_buffer)
{
	const size_type new_map_size = JStl::max(map_size_ * 2,
		map_size_ + need_buffer + 8);
	map_pointer new_map = create_map(new_map_size);
	const size_type old_buffer = end_.node - begin_.node + 1;
	const size_type new_buffer = old_buffer + need_buffer;

	auto begin = new_map + ((new_map_size - new_buffer) / 2);
	auto mid = begin + old_buffer;
	auto end = mid + need_buffer;
	for (auto begin1 = begin, begin2 = begin_.node; begin1 != mid; ++begin1, ++begin2)
		*begin1 = *begin2;
	create_buffer(mid, end - 1);

	map_allocator::deallocate(map_, map_size_);
	map_ = new_map;
	map_size_ = new_map_size;
	begin_ = iterator(*begin + (begin_.cur - begin_.first), begin);
	end_ = iterator(*(mid - 1) + (end_.cur - end_.first), mid - 1);
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::require_capacity(size_type n, bool isfront)
{
	//所需内存不够
	if (isfront && (static_cast<size_type>(begin_.cur - begin_.first) < n))
	{
		//计算所需的数目
		const size_type need_buffer = (n - (begin_.cur - begin_.first)) / buffer_size + 1;
		//如果map_中所剩的数组指针不足
		if (need_buffer > static_cast<size_type>(begin_.node - map_))
		{
			//重新分配map
			reallocate_map_at_front(need_buffer);
			return;
		}
		create_buffer(begin_.node - need_buffer, begin_.node - 1);
	}
	else if (!isfront && (static_cast<size_type>(end_.last - end_.cur - 1) < n))
	{
		const size_type need_buffer = (n - (end_.last - end_.cur - 1)) / buffer_size + 1;
		if (need_buffer > static_cast<size_type>(map_ + map_size_ - end_.node - 1))
		{
			reallocate_map_at_back(need_buffer);
			return;
		}
		create_buffer(end_.node + 1, end_.node + need_buffer);
	}
} 

template<typename T, typename Alloc = allocator<T>>
template <class... Args>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::insert_aux(iterator pos, Args&& ...args)
{
	value_type value = value_type(JStl::forward<Args>(args)...);
	const size_type elems_before = pos - begin_;
	if (elems_before < (size() / 2)){
		//放于前边
		emplace_front(value);
		move(begin_ + 1, pos, begin_);
		data_allocator::construct((--pos).cur, value);
		//这个pos一定要加括号，要不然无法换队列
	}
	else{
		emplace_back(value);
		move_backward(pos, end_ - 1, end_);
		data_allocator::construct(pos.cur, value);
	}
	return pos;
}

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
deque<T, Alloc>::deque(std::initializer_list<T> l)
{
	copy_init(l.begin(), l.end(), JStl::forward_iterator_tag());
}

template<typename T, typename Alloc = allocator<T>>
deque<T, Alloc>::deque(const deque& rhs)
{
	copy_init(rhs.begin_, rhs.end_, JStl::forward_iterator_tag());
}

template<typename T, typename Alloc = allocator<T>>
deque<T, Alloc>::deque(deque &&rhs) : begin_(JStl::move(rhs.begin_)),
									  end_(JStl::move(rhs.end_)),
									  map_(JStl::move(rhs.map_)),
									  map_size_(JStl::move(rhs.map_size_))
{
	rhs.map_ = nullptr;
	rhs.map_size_ = 0;
}

template<typename T, typename Alloc = allocator<T>>
deque<T, Alloc>& 
deque<T, Alloc>::operator=(const deque& rhs)
{
	if (this != &rhs){
		const auto len = size();
		if (len >= rhs.size()){
			erase(JStl::copy(rhs.begin_, rhs.end_, begin_), end_);
		}
		else{
			iterator mid = rhs.begin() + static_cast<difference_type>(len);
			JStl::copy(rhs.begin_, mid, begin_);
			insert(end_, mid, rhs.end_);
		}
	}
	return *this;
}

template<typename T, typename Alloc = allocator<T>>
deque<T, Alloc>&
deque<T, Alloc>::operator=(deque&& rhs)
{
	if (&rhs != this){
		clear();
		begin_ = JStl::move(rhs.begin_);
		end_ = JStl::move(rhs.end_);
		map_ = rhs.map_;
		map_size_ = rhs.map_size_;
		rhs.map_ = nullptr;
		rhs.map_size_ = 0;
	}
	return *this;
}

template<typename T, typename Alloc = allocator<T>>
deque<T, Alloc>&
deque<T, Alloc>::operator=(std::initializer_list<value_type> l)
{
	deque tmp(l);
	swap(tmp);
	return *this;
}

template<typename T, typename Alloc = allocator<T>>
deque<T, Alloc>::~deque()
{
	clear();
	data_allocator::deallocate(*begin_.node, buffer_size);
	*begin_.node = nullptr;
	map_allocator::deallocate(map_, map_size_);
	map_ = nullptr;
	map_size_ = 0;
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::shrink_to_fit() 
{
	//将map中所有空队列释放
	for (auto cur = map_; cur < begin_.node; ++cur){
		data_allocator::deallocate(*cur, buffer_size);
		*cur = nullptr;
	}
	for (auto cur = end_.node + 1; cur < map_ + map_size_; ++cur){
		data_allocator::deallocate(*cur, buffer_size);
		*cur = nullptr;
	}
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::assign(size_type n, const value_type& value)
{
	fill_assign(n, value);
}

template<typename T,typename Alloc = allocator<T>>
template <class Iter, typename std::enable_if<
	JStl::is_input_iterator<Iter>::value, int>::type = 0>
void deque<T, Alloc>::assign(Iter first, Iter last)
{
	copy_assign(first, last, iterator_category(first));
}

template<typename T, typename Alloc = allocator<T>>
template <class ...Args>
void deque<T, Alloc>::emplace_front(Args&& ...args)
{
	if (begin_.cur != begin_.first){
		--begin_;
		data_allocator::construct(begin_.cur, JStl::forward<Args>(args)...);	
	}
	else{
		require_capacity(1, true);
		--begin_;
		data_allocator::construct(begin_.cur, JStl::forward<Args>(args)...);
	}
}

template<typename T, typename Alloc = allocator<T>>
template <class ...Args>
void deque<T, Alloc>::emplace_back(Args&& ...args)
{
	//数组a[0]到a[255] end.last指向256 end.cur指向255，且未被构造
	if (end_.cur != end_.last - 1){
		data_allocator::construct(end_.cur, JStl::forward<Args>(args)...);
	}
	else{
		require_capacity(1, false);
		data_allocator::construct(end_.cur, JStl::forward<Args>(args)...);
	}
	++end_;
}

template<typename T, typename Alloc = allocator<T>>
template <class ...Args>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::emplace(iterator pos, Args&& ...args)
{
	if (pos.cur == begin_.cur){
		emplace_front(JStl::forward<Args>(args)...);
		return begin_;
	}
	else if (pos.cur == end_.cur){
		emplace_back(JStl::forward<Args>(args)...);
		return end_;
	}
	return insert_aux(pos, JStl::forward<Args>(args)...);
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::push_front(const value_type& value)
{
	if (begin_.cur != begin_.first){
		--begin_;
		data_allocator::construct(begin_.cur, value);
	}
	else{
		require_capacity(1, true);
		--begin_;
		*begin_ = value;
	}
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::push_front(value_type&& value)
{
	emplace_front(value);
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::push_back(const value_type& value)
{
	if (end_.cur != end_.last - 1){
		*end_ = value;
	}
	else{
		require_capacity(1, false);
		data_allocator::construct(end_.cur, value);
	}
	++end_;
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::push_back(value_type&& value)
{
	emplace_back(value);
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::pop_front()
{
	if (!empty()){
		data_allocator::destroy(begin_.cur);
		++begin_;
	}
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::pop_back()
{
	if (!empty()){
		--end_;
		data_allocator::destroy(end_.cur);	
	}
}

template<typename T, typename Alloc = allocator<T>>
typename deque<T, Alloc>::iterator 
deque<T, Alloc>::insert(iterator pos, const value_type& value)
{
	if (pos == begin_){
		push_front(value);
		return begin_;
	}
	else if(pos == end_){
		auto tmp = end_;
		push_back(value);
		return tmp;
	}
	else{
		return insert_aux(pos, value);
	}

}

template<typename T, typename Alloc = allocator<T>>
typename deque<T, Alloc>::iterator 
deque<T, Alloc>::insert(iterator pos, value_type&& value)
{
	if (pos == begin_){
		push_front(JStl::move(value));
		return begin_;
	}
	else if (pos == end_){
		auto tmp = end_;
		push_back(JStl::move(value));
		return tmp;
	}
	else{
		return insert_aux(pos, JStl::forward<value_type>(value));
	}
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::insert(iterator pos, size_type n, const value_type& value)
{
	const size_type elems_before = pos - begin_;
	if (elems_before < (size() / 2)){
		require_capacity(n, true);
	}
	else{
		require_capacity(n, false);
	}
	for (size_type i = 0; i < n; ++i){
		insert(pos, value);
	}
}

template<typename T, typename Alloc = allocator<T>>
template <class Iter, typename std::enable_if<
	JStl::is_input_iterator<Iter>::value, int>::type = 0>
void deque<T, Alloc>::insert(iterator pos, Iter first, Iter last)
{
	const size_type n = JStl::distance(first, last);
	const size_type elems_before = pos - begin_;
	//如果插入的位置离前面进，就从前面分配内存
	if (elems_before < (size() / 2)){
		require_capacity(n, true);
	}
	else{
		require_capacity(n, false);
	}
	pos = begin_ + elems_before;
	auto cur = --last;
	for (size_type i = 0; i < n; ++i, --cur){
		pos = insert(pos, *cur);
	}
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::insert(iterator pos, std::initializer_list<value_type> l)
{
	insert(pos, l.begin(), l.end());
}

template<typename T, typename Alloc = allocator<T>>
typename deque<T, Alloc>::iterator
deque<T, Alloc>::erase(iterator pos)
{
	auto tmp = copy(pos + 1, end_, pos);
	pop_back();
	return tmp;
}

template<typename T, typename Alloc = allocator<T>>
typename deque<T,Alloc>::iterator
deque<T, Alloc>::erase(iterator first, iterator last)
{
	if (first == begin_ && last == end_){
		clear();
		return end_;
	}
	else{
		const size_type len = last - first;
		const size_type elems_before = first - begin_;
		if (elems_before < ((size() - len) / 2)){
			JStl::move_backward(begin_, first, last);
			auto new_begin = begin_ + len;
			data_allocator::destroy(begin_.cur, new_begin.cur);
			begin_ = new_begin;
		}
		else{
			JStl::move(last, end_, first);
			auto new_end = end_ - len;
			data_allocator::destroy(new_end.cur, end_.cur);
			end_ = new_end;
		}
		return begin_ + elems_before;
	}
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::resize(size_type new_size)
{
	resize(new_size, value_type());
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::resize(size_type new_size, const value_type& value)
{
	auto size = size();
	if (size = new_size){
		return;
	}
	else if(new_size > size){
		insert(end_, new_size, value);
	}
	else{
		erase(end_ - (size_ - new_size), end_);
	}
}

template<typename T, typename Alloc = allocator<T>>
void deque<T, Alloc>::clear()
{
	// 首先将中间的clean
	for (map_pointer cur = begin_.node + 1; cur < end_.node; ++cur){
		data_allocator::destroy(*cur, *cur + buffer_size);
	}
	//然后清理头尾
	if (begin_.node != end_.node){ 
		JStl::destroy(begin_.cur, begin_.last);
		JStl::destroy(end_.first, end_.cur);
	}
	else{
		JStl::destroy(begin_.cur, end_.cur);
	}
	shrink_to_fit();
	end_ = begin_;
}

template<typename T, typename Alloc = allocator<T>>
bool operator==(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)
{
	return lhs.size() == rhs.size() &&
		JStl::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T, typename Alloc = allocator<T>>
bool operator<(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)
{
	return JStl::lexicographical_compare(
		lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<typename T, typename Alloc = allocator<T>>
bool operator!=(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)
{
	return !(lhs == rhs);
}

template<typename T, typename Alloc = allocator<T>>
bool operator>(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)
{
	return rhs < lhs;
}

template<typename T, typename Alloc = allocator<T>>
bool operator<=(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)
{
	return !(rhs < lhs);
}

template<typename T, typename Alloc = allocator<T>>
bool operator>=(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)
{
	return !(lhs < rhs);
}

// 重载 mystl 的 swap
template<typename T, typename Alloc = allocator<T>>
void swap(deque<T, Alloc>& lhs, deque<T, Alloc>& rhs)
{
	lhs.swap(rhs);
}

}//namespace JStl
#endif
