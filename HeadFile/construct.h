#ifndef  JIUGESTL_HEADFILE_CONSTRUCT
#define  JIUGESTL_HEADFILE_CONSTRUCT
//构造函数
#include <new>

#include "util.h"

namespace JStl{
/**************************************************************************************/
//construct
/**************************************************************************************/

template<typename T>
void construct(T *p)
{
	//定位new,可以在p上直接构造
	::new ((void*)p) T();
}

template <class T, class Ty2>
void construct(T* ptr, const Ty2& value)
{
	::new ((void*)ptr) T(value);
}

template <class T, class... Args>
void construct(T* ptr, Args&&... args)
{
	::new ((void*)ptr) T(JStl::forward<Args>(args)...);
}

/**************************************************************************************/
//destory
/**************************************************************************************/

template <class Ty>
void destroy_one(Ty*, std::true_type) {}

template <class Ty>
void destroy_one(Ty* pointer, std::false_type)
{
	if (pointer != nullptr)
	{
		pointer->~Ty();
	}
}

template <class Ty>
void destroy(Ty* pointer)
{	////判断是否有默认析构函数或是基本类型，如果不是就显式调用析构函数
	destroy_one(pointer, std::is_trivially_destructible<Ty>{});
}

template <class ForwardIterator>
void destroy_cat(ForwardIterator, ForwardIterator, std::true_type) {}

template <class ForwardIterator>
void destroy_cat(ForwardIterator first, ForwardIterator last, std::false_type)
{
	for (; first != last; ++first)
		destroy_one(&*first,std::false_type());
}

template <class ForwardIterator>
void destroy(ForwardIterator first, ForwardIterator last)
{
	destroy_cat(first, last, std::is_trivially_destructible<typename iterator_traits<ForwardIterator>::value_type>{});
}

}//namespace JStl
#endif