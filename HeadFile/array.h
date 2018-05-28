#ifndef  JIUGESTL_HEADFILE_ARRAY
#define  JIUGESTL_HEADFILE_ARRAY
#include<cassert>

#include <initializer_list>

#include "iterator.h"
#include "uninitialized.h"
#include "allocator.h"

//包含array,对数组的简单封装

namespace JStl{

template<class T,size_t Size>
class array{

public:
	typedef     allocator<T>					data_allocator;
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
	pointer  elem_;

public:
	//构造，拷贝构造，移动构造，析构，拷贝赋值，移动赋值
	array();

	array(const array& rhs);

	array(array&& rhs);

	array& operator=(const array& rhs);

	array& operator=(array&& rhs);

	~array();

public:
	//迭代器相关操作
	iterator begin()
	{
		return elem_;
	}

	const_iterator begin() const
	{
		return elem_;
	}

	iterator end()
	{
		return elem_ + Size;
	}

	const_iterator end() const
	{
		return elem_ + Size;
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
	//简单函数
	size_type size() const 
	{
		return Size;
	}

	size_type max_size() const
	{
		return Size;
	}

	void swap(array& rhs)
	{
		JStl::swap(rhs.elem_, elem_);
	}

	reference operator[](size_type i)
	{
		if(i < Size)
			return *(elem_ + i);
		throw std::out_of_range("array_index_out_of_range");
	}

	const_reference operator[](size_type i) const
	{
		if (i < Size)
			return *(elem_ + i);
		throw std::out_of_range("array_index_out_of_range");
	}

	reference at(size_type i)
	{
		return elem_[i];
	}

	const_reference at(size_type i) const
	{
		return elem_[i];
	}

	reference front()
	{
		return elem_[0];
	}

	const_reference front() const
	{
		return elem_[0];
	}

	reference back()
	{
		return elem_[Size - 1];
	}

	const_reference back() const
	{
		return elem_[Size - 1];
	}

	pointer data()
	{
		return elem_;
	}

public:
	//复杂函数
	void assign(value_type value);

	void fill(value_type value);
};

template<typename T, size_t Size>
array<T, Size>::array() : elem_(data_allocator::allocate(Size))
{}

template<typename T, size_t Size>
array<T, Size>::array(const array& rhs) : elem_(data_allocator::allocate(Size))
{
	for (size_t i = 0; i < Size; ++i){
		data_allocator::construct(elem_,rhs[i]);
		++elem_;
	}
	elem_ -= Size;
}

template<typename T, size_t Size>
array<T, Size>& 
array<T, Size>::operator=(const array& rhs)
{
	JStl::copy(rhs.begin(), rhs.end(), elem_);
	return *this;
}

template<typename T, size_t Size>
array<T, Size>& 
array<T, Size>::operator=(array&& rhs)
{
	if (&rhs != this)
		elem_ = rhs.begin();
	return *this;
}

template<typename T, size_t Size>
array<T, Size>::~array()
{
	data_allocator::destroy(elem_, elem_ + Size);
	data_allocator::deallocate(elem_);
}

template<typename T,size_t Size>
void array<T, Size>::assign(value_type value)
{
	uninitialized_fill_n(elem_, Size, value);
}

template<typename T, size_t Size>
void array<T, Size>::fill(value_type value)
{
	uninitialized_fill_n(elem_, Size, value);
}

template<class T>
class array<T, 0>{
	typedef     allocator<T>					data_allocator;
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
	pointer  elem_;

public:
	array() : elem_(nullptr){}

	array(const array& rhs) : elem_(nullptr){}

	array(array&& rhs) : elem_(nullptr){}

	array& operator=(const array& rhs)
	{
		elem_ = nullptr;
	}

	array& operator=(array&& rhs)
	{
		:elem_(nullptr)
	}

	~array() = default;

public:
	//迭代器相关操作
	iterator begin()
	{
		return elem_;
	}

	const_iterator begin() const
	{
		return elem_;
	}

	iterator end()
	{
		return elem_;
	}

	const_iterator end() const
	{
		return elem_;
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
	//简单函数
	size_type size() const
	{
		return 0;
	}

	size_type max_size() const
	{
		return 0;
	}

	void swap(array& rhs)
	{}

	reference operator[](size_type i)
	{
		throw std::out_of_range("array_index_out_of_range");
	}

	const_reference operator[](size_type i) const
	{
		throw std::out_of_range("array_index_out_of_range");
	}

	reference at(size_type i)
	{
		throw std::out_of_range("array_index_out_of_range");
	}

	const_reference at(size_type i) const
	{
		throw std::out_of_range("array_index_out_of_range");
	}

	reference front()
	{
		throw std::out_of_range("array_index_out_of_range");
	}

	const_reference front() const
	{
		throw std::out_of_range("array_index_out_of_range");
	}

	reference back()
	{
		throw std::out_of_range("array_index_out_of_range");
	}

	const_reference back() const
	{
		throw std::out_of_range("array_index_out_of_range");
	}

	pointer data()
	{
		return elem_;
	}

public:
	//复杂函数
	void assign(value_type value){}

	void fill(value_type value){}
};

template<typename T,size_t Size>
void swap(array<T, Size> &lhs, array<T, Size> &rhs)
{
	lhs.swap(rhs);
}

template<typename T, size_t Size>
bool operator==(array<T, Size> &lhs, array<T, Size> &rhs)
{
	return JStl::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T, size_t Size>
bool operator!=(array<T, Size> &lhs, array<T, Size> &rhs)
{
	return !(lhs == rhs);
}

template<typename T, size_t Size>
bool operator<(array<T, Size> &lhs, array<T, Size> &rhs)
{
	return JStl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<typename T, size_t Size>
bool operator>(array<T, Size> &lhs, array<T, Size> &rhs)
{
	return rhs < lhs;
}

template<typename T, size_t Size>
bool operator<=(array<T, Size> &lhs, array<T, Size> &rhs)
{
	return !(lhs > rhs);
}

template<typename T, size_t Size>
bool operator>=(array<T, Size> &lhs, array<T, Size> &rhs)
{
	return !(rhs > lhs);
}


}//namespace JStl
#endif