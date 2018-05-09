#ifndef  JIUGESTL_HEADFILE_ARRAY
#define  JIUGESTL_HEADFILE_ARRAY
#include<cassert>

#include <initializer_list>

#include "iterator.h"
#include "uninitialized.h"
#include "allocator.h"

//包含array

namespace JStl{

template<class T,size_t Size>
class array{
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
	array() : elem_(data_allocator::allocate(Size))
	{}

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
};

}//namespace JStl
#endif