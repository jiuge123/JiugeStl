#ifndef  JIUGESTL_HEADFILE_VECTOR
#define  JIUGESTL_HEADFILE_VECTOR

#include<cassert>

#include <initializer_list>

#include "iterator.h"
#include "uninitialized.h"
#include "allocator.h"

//包含vector

namespace JStl{
template<typename T,typename Alloc = JStl::allocator<T>>
class vector{
public:
	typedef		Alloc							data_allocator;
	typedef		T								value_type;
	typedef		T*								pointer;
	typedef		const T*						const_pointer;
	typedef		T&					 			reference;
	typedef		const T&						const_reference;
	typedef		size_t							size_type;
	typedef		ptrdiff_t						difference_type;

	typedef		T*					iterator;
	typedef		const T*  			const_iterator;
	typedef		JStl::reverse_iterator<iterator> 
									reverse_iterator;
	typedef		JStl::reverse_iterator<const_iterator>
									const_reverse_iterator;
private:

	T *begin_;  //可用的首地址
 	T *end_;	//可用的尾地址
	T *cap_;	//容量地址
	
	//初始分配空间
	void init_space(size_t);

	//重新分配地址
	void refill_alloc_space(size_t);
	
public:
	//构造，拷贝构造，移动构造，析构，拷贝赋值，移动赋值
	vector() :begin_(0), end_(0), cap_(0){}

	vector(size_t);

	vector(size_t, const T&);

	vector(const std::initializer_list<value_type>&);

	//如果不写后面那个，只要是两个参数就会往这里匹配，比如两个int
	//enable是使用type时，如果前面的表达式为真，才能用，前面为假不会匹配
	template<typename Iter, typename std::enable_if<
		is_input_iterator<Iter>::value, int>::type = 0>
	vector(Iter first, Iter last);

	vector(const vector&);

	vector(vector&&) _NOEXCEPT;

	~vector();

	vector& operator=(const vector&) ;

	vector& operator=(vector&&) _NOEXCEPT;

	vector& operator=(const std::initializer_list<value_type>&);
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

	const_reverse_iterator rbegin() const 
	{ 
		return const_reverse_iterator(end()); 
	}

	reverse_iterator rend()          
	{ 
		return reverse_iterator(begin()); 
	}

	const_reverse_iterator rend() const 
	{ 
		return const_reverse_iterator(begin()); 
	}

	const_iterator cbegin() const 
	{ 
		return begin(); 
	}

	const_iterator cend() const 
	{ 
		return end(); 
	}

	const_reverse_iterator crbegin() const 
	{ 
		return rbegin(); 
	}

	const_reverse_iterator crend() const 
	{ 
		return rend(); 
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
	//成员函数
	void assign(size_type n, const value_type& value);
	template <class Iter, typename std::enable_if<
		JStl::is_input_iterator<Iter>::value, int>::type = 0>
	void assign(Iter first, Iter last);
	void assign(const std::initializer_list<value_type> &il);
	
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
	void insert(iterator pos, const std::initializer_list<value_type> &il);

	void resize(size_t newsize, const value_type& x);
	void reverse();
	void clear();
	void swap(vector &rhs);
	
};

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::init_space(size_t n)
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
void vector<T, Alloc>::refill_alloc_space(size_t n)
{
	n = (n == 0)? 1 : n;
	T *newp = data_allocator::allocate(n);
	if (newp == nullptr)
		throw bad_alloc();
	size_t s = size();
	uninitialized_move(begin_, end_, newp);
	data_allocator::destroy(begin_, end_);
	data_allocator::deallocate(begin_);
	begin_ = newp;
	end_ = newp + s;
	cap_ = begin_ + n;	
}

template<typename T, typename Alloc = allocator<T>>
vector<T,Alloc>::vector(size_t n, const T& value)
{
	init_space(n);
	JStl::uninitialized_fill_n(begin_, n, value);
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>::vector(size_t n)
{
	init_space(n);
	JStl::uninitialized_fill_n(begin_, n, value_type());
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>::vector(const std::initializer_list<T> &i)
{
	init_space(i.size());
	JStl::uninitialized_copy(i.begin(), i.end(), begin_);
}

template<typename T, typename Alloc = allocator<T>>
template<typename Iter, typename std::enable_if<is_input_iterator<Iter>::value, int>::type = 0>
vector<T, Alloc>::vector(Iter first, Iter last)
{	
	init_space(distance(first,last));
	JStl::uninitialized_copy(first, last, begin_);	
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>::vector(const vector &rhs)
{
	init_space(rhs.size());
	JStl::uninitialized_copy(rhs.begin(), rhs.end(), begin_);
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>::vector(vector &&rhs) :begin_(rhs.begin_), end_(rhs.end_), cap_(rhs.cap_) 
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
		init_space(rhs.size());
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
vector<T, Alloc>& vector<T, Alloc>::operator=(const std::initializer_list<value_type> &l)
{
	init_space(l.size());
	JStl::uninitialized_copy(l.begin(), l.end(), begin);
}

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::assign(size_type n, const value_type& value)
{
	clear();
	if (n > capacity()){
		begin_ = data_allocator::allocate(n);
		uninitialized_fill_n(begin_, n, value);
		cap_ = end_ = begin_ + n;
	}
	else{
		uninitialized_fill_n(begin_, n, value);
		end_ = begin_ + n;
	}
}

template<typename T, typename Alloc = allocator<T>>
template <class Iter, typename std::enable_if<
	JStl::is_input_iterator<Iter>::value, int>::type = 0>
	void vector<T, Alloc>::assign(Iter first, Iter last)
{
		size_t n = distance(first, last);
		clear();
		if (n > capacity()){
			begin_ = data_allocator::allocate(n);
			uninitialized_copy(first, last, begin_);
			cap_ = end_ = begin_ + n;
		}
		else{
			uninitialized_copy(first, last, begin_);
			end_ = begin_ + n;
		}
	}

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::assign(const std::initializer_list<value_type> &il)
{
	size_t n = il.size();
	clear();
	if (n > capacity()){
		begin_ = data_allocator::allocate(n);
		uninitialized_copy(il.begin(), il.end(), begin_);
		cap_ = end_ = begin_ + n;
	}
	else{
		uninitialized_copy(il.begin(), il.end(), begin_);
		end_ = begin_ + n;
	}
}

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::push_back(const value_type& value)
{
	if (size() == capacity()){
		refill_alloc_space(capacity() * 2);
	}
	data_allocator::construct(end_++, value);
}

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::push_back(value_type&& value)
{
	if (size() == capacity()){
		refill_alloc_space(capacity() * 2);
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
		data_allocator::construct(pos, JStl::forward<Args&&>(args)...);
		++end_;
	}
	else if (size() != capacity()){					//不在结尾有位置
		move_backward(pos, end_, end_ + 1);
		*pos = value_type(JStl::forward<Args&&>(args)...);
		++end_;
	}
	else {											//无位置
		refill_alloc_space(capacity() * 2);
		return emplace(begin_ + n, JStl::forward<Args&&>(args)...);
	}
	return begin_ + n;
}

template<typename T, typename Alloc = allocator<T>>
template <class... Args>
void vector<T, Alloc>::emplace_back(Args&& ...args)
{
	if (size() != capacity()){       
		data_allocator::construct(end(), JStl::forward<Args&&>(args)...);
		++end_;
	}
	else {											
		refill_alloc_space(capacity() * 2);
		emplace_back(JStl::forward<Args&&>(args)...);
	}
}

template<typename T, typename Alloc = allocator<T>>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(iterator pos)
{
	assert(pos >= begin_ && pos <= end_);
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
	assert(first >= begin_ && first <= end_);
	assert(last >= begin_ && last <= end_);
	assert(first <= last);
	iterator it = uninitialized_move(last, end_, first);
	data_allocator::destroy(it, end_);
	end_ = it;
	return first;
}

template<typename T, typename Alloc = allocator<T>>
typename vector<T, Alloc>::iterator 
vector<T, Alloc>::insert(iterator pos, const value_type& value)
{
	assert(pos >= begin_ && pos <= end_);
	size_t n = pos - begin_;
	if (size() != capacity() && pos == end_){       //在结尾有位置
		data_allocator::construct(pos, value);
		++end_;
	}
	else if (size() != capacity()){					//不在结尾有位置
		move_backward(pos, end_, end_ + 1);
		*pos = value;
		++end_;
	}
	else {											//无位置
		refill_alloc_space(capacity() * 2);
		return insert(begin_ + n, value);
	}
	return begin_ + n;
	
}

template<typename T, typename Alloc = allocator<T>>
typename vector<T, Alloc>::iterator 
vector<T, Alloc>::insert(iterator pos, value_type&& value)
{
	assert(pos >= begin_ && pos <= end_);
	size_t n = pos - begin_;
	if (size() != capacity() && pos == end_){       //在结尾有位置
		data_allocator::construct(pos, JStl::move(value));
		++end_;
	}
	else if (size() != capacity()){					//不在结尾有位置
		move_backward(pos, end_, end_ + 1);
		*pos = value;
		++end_;
	}
	else {											//无位置
		refill_alloc_space(capacity() * 2);
		return insert(begin_ + n, JStl::move(value));
	}
	return begin_ + n;
}

template<typename T, typename Alloc = allocator<T>>
typename vector<T, Alloc>::iterator 
vector<T, Alloc>::insert(iterator pos, size_type n, const value_type& value)
{
	size_t pos_n = pos - begin_;
	if (n != 0){
		size_t pos_after = end_ - pos;
		if ((size_t)(cap_ - end_) >= n){ //空闲空间大于n      
			move_backward(pos, end_, end_ + n);
			fill_n(pos, n, value);
			end_ = end_ + n;
		}
		else{
			size_t len = (size() > n ? size() : n) + size(); //size 和 n 大的作为新size
			auto new_begin = data_allocator::allocate(len);
			if (new_begin == 0)
				throw bad_alloc();
			auto new_end = new_begin;
			new_end = uninitialized_move(begin_, pos, new_begin);
			new_end = uninitialized_fill_n(new_end, n, value);
			new_end = uninitialized_move(pos, end_, new_end);
			data_allocator::deallocate(begin_);
			begin_ = new_begin;
			end_ = new_end;
			cap_ = begin_ + len;			
		}
		return begin_ + pos_n;
	}
}

template<typename T, typename Alloc = allocator<T>>
template <class InputIterator, typename std::enable_if<
	JStl::is_input_iterator<InputIterator>::value, int>::type = 0>
void vector<T, Alloc>::insert(iterator pos, InputIterator first, InputIterator last)
{
	if (first == last)
		return;
	size_t n = JStl::distance(first, last);
	if (n != 0){
		if ((size_t)(cap_ - end_) >= n){ //空闲空间大于n
			copy_backward(pos, end_, end_ + n);
			uninitialized_copy(first, last, pos);
			end_ = end_ + n;
		}
		else{
			size_t len = (size() > n ? size() : n) + size(); //size 和 n 大的作为新size
			auto new_begin = data_allocator::allocate(len);
			if (new_begin == 0)
				throw bad_alloc();
			auto new_end = new_begin;
			new_end = uninitialized_move(begin_, pos, new_begin);
			new_end = uninitialized_copy(first, last, new_end);
			new_end = uninitialized_move(pos, end_, new_end);
			data_allocator::deallocate(begin_);
			begin_ = new_begin;
			end_ = new_end;
			cap_ = begin_ + len;
		}
	}
}

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::insert(iterator pos, const std::initializer_list<value_type> &il)
{
	size_t n = il.size();
	if (n != 0){
		if ((size_t)(cap_ - end_) >= n){ //空闲空间大于n
			move_backward(pos, end_, end_ + n);
			uninitialized_copy(il.begin(), il.end(), pos);
			end_ = end_ + n;
		}
		else{
			size_t len = (size() > n ? size() : n) + size(); //size 和 n 大的作为新size
			auto new_begin = data_allocator::allocate(len);
			if (new_begin == 0)
				throw bad_alloc();
			auto new_end = new_begin;
			new_end = uninitialized_move(begin_, pos, new_begin);
			new_end = uninitialized_copy(il.begin(), il.end(), new_end);
			new_end = uninitialized_move(pos, end_, new_end);
			data_allocator::deallocate(begin_);
			begin_ = new_begin;
			end_ = new_end;
			cap_ = begin_ + len;
		}
	}
}

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::resize(size_t newsize, const value_type& x)
{
	if (newsize < size()){
		erase(begin_ + newsize, end_);
	}	
	else{
		insert(end_, newsize - size(), x);
	}	
}

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::clear()
{
	erase(begin_, end_);
}

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::reverse()
{
	value_type temp;
	for (size_t i = 0; i < size()/2; ++i){
		temp = move(*(begin_ + i));
		*(begin_ + i) = move(*(begin_ + size() - i - 1));
		*(begin_ + size() - i - 1) = move(temp);
	}
}

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::swap(vector &rhs)
{
	vector temp(move(rhs));
	rhs = move(*this);
	*this = move(temp);
}

//重载运算符
template<typename T, typename Alloc = allocator<T>>
bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
{
	if (lhs.size() != rhs.size())
		return 0;
	return equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T, typename Alloc = allocator<T>>
bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
{
	return !(lhs == rhs);
}

template<typename T, typename Alloc = allocator<T>>
bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
{
	return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<typename T, typename Alloc = allocator<T>>
bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
{
	return rhs < lhs;
}

template<typename T, typename Alloc = allocator<T>>
bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
{
	return !(lhs < rhs);
}

template<typename T, typename Alloc = allocator<T>>
bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
{
	return !(rhs > lhs);
}


}//namespaec JStl;
#endif