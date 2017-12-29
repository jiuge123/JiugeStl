#ifndef  JIUGESTL_HEADFILE_VECTOR
#define  JIUGESTL_HEADFILE_VECTOR

#include<cassert>

#include <initializer_list>

#include "iterator.h"
#include "uninitialized.h"
#include "allocator.h"

namespace JStl{
template<typename T,typename Alloc = JStl::allocator<T>>
class vector{
public:
	typedef		Alloc				data_allocator;
	typedef		T					value_type;
	typedef		T*					pointer;
	typedef		const T*			const_pointer;
	typedef		T&					reference;
	typedef		const T&			const_reference;
	typedef		size_t			    size_type;
	typedef		ptrdiff_t			difference_type;

	typedef		T*					iterator;
	typedef		const T*			const_iterator;
	typedef		JStl::reverse_iterator<iterator> 
									reverse_iterator;
	typedef		JStl::reverse_iterator<const_iterator>
									const_reverse_iterator;
private:

	T *begin_;  //可用的首地址
 	T *end_;	//可用的尾地址
	T *cap_;	//容量地址
	
	//初始分配空间
	void init_Space(size_t);

	//重新分配地址
	void fill_Alloc_Space(size_t);
	
public:
	//构造，拷贝构造，移动构造，析构，拷贝赋值，移动赋值
	vector() :begin_(0), end_(0), cap_(0){}

	vector(size_t, const T&);

	vector(std::initializer_list<value_type>);

	//如果不写后面那个，只要是两个参数就会往这里匹配，比如两个int
	//enable是使用type时，如果前面的表达式为真，才能用，前面未假不会匹配
	template<typename Iter, typename std::enable_if<is_input_iterator<Iter>::value, int>::type = 0>
	vector(Iter first, Iter last);

	vector(const vector&);

	vector(vector&&) _NOEXCEPT;

	~vector();

	vector& operator=(const vector&) ;

	vector& operator=(vector&&) _NOEXCEPT;

	vector& operator=(std::initializer_list<value_type>);
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

public:
	//普通函数
	reference operator[](size_t i)
	{
		if (i < size())
			return *(begin_ + i);
		throw std::out_of_range("vector_index_out_of_range");
	}

	const_reference operator[](size_t i) const
	{
		if (i < size())
			return *(begin_ + i);
		throw std::out_of_range("vector_index_out_of_range");
	}

	reference at(size_t i)
	{
		if (i < size())
			return *(begin_ + i);
		throw std::out_of_range("vector_index_out_of_range");
	}

	const_reference at(size_t i) const
	{
		if (i < size())
			return *(begin_ + i);
		throw std::out_of_range("vector_index_out_of_range");
	}

	reference back()
	{
		return *(end_ - 1);
	}

	const_reference back() const
	{
		return *(end_ - 1);
	}

	reference front()
	{
		return *begin_;
	}

	const_reference front() const
	{
		return *begin_;
	}

	//返回vector元素数目
	size_t size() const
	{
		return end_ - begin_;
	}

	//返回vector容量
	size_t capacity() const
	{
		return cap_ - begin_;
	}

	bool empty() const
	{
		return begin_ == end_;
	}

public:
	void push_back(const value_type& value);
	void push_back(value_type&& value);
	
	void pop_back();

	template <class... Args>
	iterator emplace(iterator pos, Args&& ...args);
	template <class... Args>
	void emplace_back(Args&& ...args);

	iterator erase(iterator pos);
	iterator erase(iterator first, iterator last);

	iterator insert(iterator pos, const value_type& value);
	iterator insert(iterator pos, value_type&& value);
	iterator insert(iterator pos, size_type n, const value_type& value);
	template <class Iter, typename std::enable_if<JStl::is_input_iterator<Iter>::value, int>::type = 0>
	void insert(iterator pos, Iter first, Iter last);

	void resize(size_t newsize, const value_type& x);
};

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::init_Space(size_t n)
{
	try{
		begin_ = data_allocator::allocate(n);
		end_ = begin_ + n;
		cap_ = end_;
	}
	catch (...){
		begin_ = nullptr;
		end_ = nullptr;
		cap_ = nullptr;
	}
}

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::fill_Alloc_Space(size_t n)
{
	n = (n == 0)? 1 : n;
	T *newp = data_allocator::allocate(n);
	if (newp == nullptr)
		throw bad_alloc();
	size_t s = size();
	uninitialized_copy(begin_, end_, newp);
	data_allocator::destroy(begin_, end_);
	data_allocator::deallocate(begin_);
	begin_ = newp;
	end_ = newp + s;
	cap_ = begin_ + n;	
}

template<typename T, typename Alloc = allocator<T>>
vector<T,Alloc>::vector(size_t n, const T& value)
{
	init_Space(n);
	JStl::uninitialized_fill_n(begin_, n, value);
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>::vector(std::initializer_list<T> i)
{
	init_Space(i.size());
	JStl::uninitialized_copy(i.begin(), i.end(), begin_);
}

template<typename T, typename Alloc = allocator<T>>
template<typename Iter, typename std::enable_if<is_input_iterator<Iter>::value, int>::type = 0>
vector<T, Alloc>::vector(Iter first, Iter last)
{
	if (first < last){
		init_Space(last - first);
		JStl::uninitialized_copy(first, last, begin_);
	}	
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>::vector(const vector &rhs)
{
	init_Space(rhs.size());
	JStl::uninitialized_copy(rhs.begin(), rhs.end(), begin_);
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>::vector(vector &&rhs) :begin_(rhs.begin_), end_(rhs.end_), cap_(rhs.cap_) _NOEXCEPT
{
	rhs.begin_ = nullptr;
	rhs.end_ = nullptr;
	rhs.cap_ = nullptr;
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>::~vector()
{
	data_allocator::destroy(begin_, end_);
	data_allocator::deallocate(begin_);
	begin_ = nullptr;
	end_ = nullptr;
	cap_ = nullptr;
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>& vector<T, Alloc>::operator=(const vector &rhs)
{
	if (begin_ == rhs.begin_){
		return *this;
	}
	else{
		this->~vector();
		init_Space(rhs.size());
		JStl::uninitialized_copy(rhs.begin_, rhs.end_, begin_);
		return *this;
	}	
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>& vector<T, Alloc>::operator=(vector &&rhs) _NOEXCEPT
{
	if (begin_ == rhs.begin_){
		return *this;
	}
	else{
		begin_ = rhs.begin_;
		end_ = rhs.end_;
		cap_ = rhs.cap_;
		rhs.begin_ = nullptr;
		rhs.end_ = nullptr;
		rhs.cap_ = nullptr;
		return *this;
	}
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>& vector<T, Alloc>::operator=(std::initializer_list<value_type> l)
{
	init_Space(l.size());
	JStl::uninitialized_copy(l.begin(), l.end(), begin);
}

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::push_back(const value_type& value)
{
	if (size() == capacity()){
		fill_Alloc_Space(capacity() * 2);
	}
	data_allocator::construct(end_++, value);
}

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::push_back(value_type&& value)
{
	if (size() == capacity()){
		fill_Alloc_Space(capacity() * 2);
	}
	data_allocator::construct(end_++, value);
}

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::pop_back()
{
	--end_;
	data_allocator::destroy(end_);
}

//在pos就地构造函数
template<typename T, typename Alloc = allocator<T>>
template <class... Args> 
typename vector<T, Alloc>::iterator 
vector<T, Alloc>::emplace(iterator pos, Args&& ...args)
{
	assert(pos >= begin_ && pos <= end_);
	size_t n = pos - begin_;
	if (size() != capacity() && pos == end_){       //在结尾有位置
		data_allocator::construct(pos, JStl::forward<Args>(args)...);
		++end_;
	}
	else if (size() != capacity()){					//不在结尾有位置
		copy_backward(pos, end_, end_ + 1);
		*pos = value_type(JStl::forward<Args>(args)...);
		++end_;
	}
	else {											//无位置
		fill_Alloc_Space(capacity() * 2);
		return emplace(begin_ + n, JStl::forward<Args>(args)...);
	}
	return begin_ + n;
}

template<typename T, typename Alloc = allocator<T>>
template <class... Args>
void vector<T, Alloc>::emplace_back(Args&& ...args)
{
	if (size() != capacity()){       
		data_allocator::construct(pos, JStl::forward<Args>(args)...);
		++end_;
	}
	else {											
		fill_Alloc_Space(capacity() * 2);
		emplace_back(JStl::forward<Args>(args)...);
	}
}

template<typename T, typename Alloc = allocator<T>>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(iterator pos)
{
	if (pos + 1 != end_)
		uninitialized_copy(pos + 1, end_, pos);
	--end_;
	data_allocator::destroy(end_);
	return pos;
}

template<typename T, typename Alloc = allocator<T>>
typename vector<T, Alloc>::iterator 
vector<T, Alloc>::erase(iterator first, iterator last)
{
	iterator it = uninitialized_copy(last, end_, first);
	data_allocator::destroy(it, end_);
	end_ = it;
	return first;
}

template<typename T, typename Alloc = allocator<T>>
typename vector<T, Alloc>::iterator 
vector<T, Alloc>::insert(iterator pos, const value_type& value)
{

}

template<typename T, typename Alloc = allocator<T>>
typename vector<T, Alloc>::iterator 
vector<T, Alloc>::insert(iterator pos, value_type&& value)
{

}

template<typename T, typename Alloc = allocator<T>>
typename vector<T, Alloc>::iterator 
vector<T, Alloc>::insert(iterator pos, size_type n, const value_type& value)
{

}

template<typename T, typename Alloc = allocator<T>>
template <class Iter, typename std::enable_if<
	JStl::is_input_iterator<Iter>::value, int>::type = 0>
	void vector<T, Alloc>::insert(iterator pos, Iter first, Iter last)
{

}

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::resize(size_t newsize, const value_type& x)
{
	
}


}//namespaec JStl;
#endif