#ifndef  JIUGESTL_HEADFILE_VECTOR
#define  JIUGESTL_HEADFILE_VECTOR

#include <initializer_list>

#include "iterator.h"
#include "uninitialized.h"

namespace JStl{
template<typename T,typename Alloc = allocator<T>>
class vector{ 
	static Alloc data_allocator;
	typedef		T					value_type;
	typedef		T*					pointer;
	typedef		const T*			const_pointer;
	typedef		T&					reference;
	typedef		const T&			const_reference;
	typedef		size_t			    size_type;
	typedef		ptrdiff_t			difference_type;
private:
	T *begin_;  //可用的首地址
 	T *end_;	//可用的尾地址
	T *cap_;	//容量地址
	
	//初始分配空间
	void init_Space(size_t);

	//初始填充
	void fill_init(size_t, const T&);
		
public:
	vector() :begin_(0), end_(0), cap(0){}

	vector(size_t, const T&);
};



template<typename T, typename Alloc = allocator<T>>
vector<T,Alloc>::vector(size_t n, const T& value)
{
	fill_init(n, value);
}

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::fill_init(size_t n, const T& value)
{
	init_Space(n);
	JStl::uninitialized_fill_n(begin_, n, value);
}


template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::init_Space(size_t n)
{
	try{
		begin_ = data_allocator.allocate(n);
		end_ = begin_ + n;
		cap_ = end_;
	}
	catch (...){
		begin_ = nullptr;
		end_ = nullptr;
		cap_ = nullptr;
	}
}



}//namespaec JStl;
#endif